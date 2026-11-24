"""Generate QuadShield.kicad_pcb: placement, routing, GND pours.

Run with the Python bundled with KiCad 8 (has pcbnew + numpy):
  "%LOCALAPPDATA%\\Programs\\KiCad\\8.0\\bin\\python.exe" gen_pcb.py

Board coordinates in this file: mm, origin = top-left corner of the shield,
x to the right, y down, seen from the TOP (the DISC1 sits on top, CN1 USB at
the top edge). The DISC1 outline is y 0 .. 119.3; below it an extension
holds the tall parts (XT30, buzzer, ESC / RC / GPS headers).

Routing: a simple 2-layer grid A* router (0.127 mm grid = 2.54 / 20, so the
channels between header pins line up), 8 directions, vias between layers.
GND is not routed: it connects through copper pours on both layers plus
stitching vias. Check the result with DRC (kicad-cli pcb drc).
"""
import heapq
import math
import os
import sys

import numpy as np
import pcbnew

import design
from gen_sch import uid

FP_DIR = "C:/Users/Lenovo/AppData/Local/Programs/KiCad/8.0/share/kicad/footprints"
OUT = "QuadShield.kicad_pcb"
OX, OY = 100.0, 40.0                     # board origin in KiCad page coords
W, H = 66.0, 155.0                       # shield size (DISC1 is 66 x 119.3)
DISC1_H = 119.30

CLEAR = 0.2                              # copper clearance
EDGE_CLEAR = 0.4
VIA_D, VIA_DRILL = 0.6, 0.3
GRID = 0.127
WIDTH = {"SW": 0.8, "+5V_BUCK": 0.8, "+5V": 0.6, "VBAT": 0.6, "VBAT_F": 0.8,
         "VBAT_RAW": 0.8, "BST": 0.3}
SIG_W = 0.25

# ---------------------------------------------------------------------------
# Placement: ref -> (x, y, rotation_deg) of the footprint anchor.
# Headers: anchor = pin 1. SMD: anchor = centre.
# ---------------------------------------------------------------------------
P1_X, P2_X, ROW1_Y = 6.33, 57.13, 33.82    # UM1670 fig. 5
EXT = DISC1_H                               # extension starts here
PLACE = {
    "J1": (P1_X, ROW1_Y, 0),               # pin2 at +x, pin3 at +y
    "J2": (P2_X, ROW1_Y, 0),

    # --- power section, top area above the sockets (y 2 .. 30) ---
    "C1": (21.0, 12.0, 90),
    "C2": (24.0, 12.0, 90),
    "U1": (28.5, 12.0, 0),
    "C3": (28.5, 7.5, 0),
    "L1": (35.5, 12.0, 0),
    "C4": (41.5, 10.5, 90),
    "C5": (44.5, 10.5, 90),
    "D2": (41.5, 19.0, 180),
    "C6": (35.5, 22.5, 0),
    "R1": (48.5, 8.0, 0),
    "D3": (48.5, 11.0, 180),

    # --- battery sense, next to J2 pin 15 (PC3) ---
    "R2": (50.0, 46.0, 90),
    "R3": (50.0, 52.0, 90),
    "C7": (47.0, 52.0, 90),

    # --- MPU (GY-521 soldered flat), centre of the board ---
    "J8": (24.11, 98.0, 90),               # 90 deg: pins along +x

    # --- I2C pull-ups (DNP) ---
    "R6": (40.0, 92.0, 0),
    "R7": (40.0, 94.5, 0),

    # --- extension: left zone ---
    "J4": (3.0, EXT + 5.0, 90),           # ESC M1, pins along +x
    "J5": (13.0, EXT + 5.0, 90),          # ESC M2
    "J10": (3.0, EXT + 12.0, 90),         # RC1
    "J11": (13.0, EXT + 12.0, 90),        # RC2
    "J3": (0, 0, 0),                       # XT30: computed below
    "F1": (25.0, EXT + 27.0, 90),
    "D1": (29.0, EXT + 27.0, 90),

    # --- extension: right zone ---
    "J6": (45.5, EXT + 5.0, 90),          # ESC M3
    "J7": (55.5, EXT + 5.0, 90),          # ESC M4
    "J12": (45.5, EXT + 12.0, 90),        # RC3
    "J13": (55.5, EXT + 12.0, 90),        # RC4
    "J14": (45.5, EXT + 19.0, 90),        # RC5
    "J9": (63.0, EXT + 17.5, 0),           # GPS, pins along +y
    "BZ1": (47.0, EXT + 29.0, 0),

    # --- buzzer driver, centre strip of the extension (SMD only: CN6 cable) ---
    "Q1": (39.5, EXT + 27.0, 0),
    "R4": (35.0, EXT + 25.5, 0),
    "R5": (35.0, EXT + 28.5, 0),
    "D4": (39.5, EXT + 31.5, 0),
}


def schematic_nets():
    """(ref, pin) -> net name exactly as in the schematic netlist
    (local labels are "/NAME", unused pins "unconnected-(...)")."""
    from sexpr import parse, find, find_all
    n = parse(open("QuadShield.net", encoding="utf-8").read())
    out = {}
    for netn in find_all(find(n, "nets"), "net"):
        name = find(netn, "name")[1]
        for node in find_all(netn, "node"):
            out[(find(node, "ref")[1], find(node, "pin")[1])] = name
    return out


def base(name):
    """"/+5V" -> "+5V" (design.py names)."""
    return name[1:] if name.startswith("/") else name


def mm(v):
    return pcbnew.ToMM(v)


def pt(x, y):
    return pcbnew.VECTOR2I(pcbnew.FromMM(OX + x), pcbnew.FromMM(OY + y))


def build():
    board = pcbnew.CreateEmptyBoard()
    ds = board.GetDesignSettings()
    ds.SetCopperLayerCount(2)
    nc = ds.m_NetSettings.m_DefaultNetClass
    nc.SetClearance(pcbnew.FromMM(CLEAR))
    nc.SetTrackWidth(pcbnew.FromMM(SIG_W))
    nc.SetViaDiameter(pcbnew.FromMM(VIA_D))
    nc.SetViaDrill(pcbnew.FromMM(VIA_DRILL))
    ds.m_MinClearance = pcbnew.FromMM(CLEAR)
    ds.m_TrackMinWidth = pcbnew.FromMM(0.2)
    ds.m_CopperEdgeClearance = pcbnew.FromMM(0.3)
    ds.m_MinResolvedSpokes = 1
    ds.m_HoleToHoleMin = pcbnew.FromMM(0.25)

    nets = {}

    def net(name):
        if name not in nets:
            n = pcbnew.NETINFO_ITEM(board, name)
            board.Add(n)
            nets[name] = n
        return nets[name]

    sch_net = schematic_nets()
    fps = {}
    for ref, (val, sym, fp, pins, lcsc, note) in design.PARTS.items():
        lib, name = fp.split(":")
        f = pcbnew.FootprintLoad(f"{FP_DIR}/{lib}.pretty", name)
        f.SetFPID(pcbnew.LIB_ID(lib, name))
        f.SetReference(ref)
        f.SetValue(val)
        f.SetPath(pcbnew.KIID_PATH("/" + uid("sym", ref)))
        if "DNP" in val:
            f.SetDNP(True)
        x, y, rot = PLACE[ref]
        f.SetOrientationDegrees(rot)
        f.SetPosition(pt(x, y))
        board.Add(f)
        # Small parts: reference on the Fab layer (assembly drawing) instead of
        # the silkscreen, which has no room for it; connectors keep silk labels
        if ref[0] in "RCDLQUF" or ref == "J3":
            f.Reference().SetLayer(pcbnew.F_Fab)
        for p in f.Pads():
            num = p.GetNumber()
            if (ref, num) in sch_net:
                p.SetNet(net(sch_net[(ref, num)]))
        fps[ref] = f

    # XT30: mouth flush with the bottom edge, pins inside
    f = fps["J3"]
    f.SetOrientationDegrees(180)
    f.SetPosition(pt(10.0, H - 12.0))
    bb = f.GetBoundingBox(False, False)
    shift = (OY + H - 0.5) - mm(bb.GetBottom())
    f.SetPosition(pt(10.0, H - 12.0 + shift))

    # Self-check of the DISC1 socket orientation (UM1670 fig. 5)
    for ref, x0 in (("J1", P1_X), ("J2", P2_X)):
        pads = {p.GetNumber(): p.GetPosition() for p in fps[ref].Pads()}
        p1, p2, p3 = pads["1"], pads["2"], pads["3"]
        assert abs(mm(p2.x - p1.x) - 2.54) < 0.01 and abs(mm(p3.y - p1.y) - 2.54) < 0.01, ref
        assert abs(mm(p1.x) - OX - x0) < 0.01

    # Board outline + DISC1 outline on silkscreen / user layer
    def rect(layer, x0, y0, x1, y1, w):
        for (a, b), (c, d) in (((x0, y0), (x1, y0)), ((x1, y0), (x1, y1)), ((x1, y1), (x0, y1)), ((x0, y1), (x0, y0))):
            s = pcbnew.PCB_SHAPE(board)
            s.SetShape(pcbnew.SHAPE_T_SEGMENT)
            s.SetStart(pt(a, b))
            s.SetEnd(pt(c, d))
            s.SetLayer(layer)
            s.SetWidth(pcbnew.FromMM(w))
            board.Add(s)
    rect(pcbnew.Edge_Cuts, 0, 0, W, H, 0.1)
    rect(pcbnew.F_SilkS, 0.3, 0.3, W - 0.3, DISC1_H, 0.12)        # DISC1 outline

    def text(t, x, y, size=1.0, layer=pcbnew.F_SilkS):
        tx = pcbnew.PCB_TEXT(board)
        tx.SetText(t)
        tx.SetPosition(pt(x, y))
        tx.SetLayer(layer)
        tx.SetTextSize(pcbnew.VECTOR2I(pcbnew.FromMM(size), pcbnew.FromMM(size)))
        tx.SetTextThickness(pcbnew.FromMM(size * 0.15))
        board.Add(tx)

    text("NOSE ^  QuadShield rev A", 33.0, 3.0, 1.2)
    text("DISC1 sits on top (CN1 USB this side)", 33.0, 27.5, 0.9)
    text("keep CN6 cable area free", 33.0, EXT + 3.0, 0.8)
    text("M1  S . -", 5.5, EXT + 1.7, 0.8)
    text("M2  S . -", 15.5, EXT + 1.7, 0.8)
    text("M3  S . -", 48.0, EXT + 1.7, 0.8)
    text("M4  S . -", 58.0, EXT + 1.7, 0.8)
    text("RC1 S + -", 5.5, EXT + 8.7, 0.8)
    text("RC2 S + -", 15.5, EXT + 8.7, 0.8)
    text("RC3 S + -", 48.0, EXT + 8.7, 0.8)
    text("RC4 S + -", 58.0, EXT + 8.7, 0.8)
    text("RC5 S + -", 48.0, EXT + 15.7, 0.8)
    # J9 reference already marks the GPS header
    gl = pcbnew.PCB_TEXT(board)
    gl.SetText("5V TX RX SCL SDA G")
    gl.SetPosition(pt(60.6, EXT + 24.0))
    gl.SetLayer(pcbnew.F_SilkS)
    gl.SetTextSize(pcbnew.VECTOR2I(pcbnew.FromMM(0.8), pcbnew.FromMM(0.8)))
    gl.SetTextThickness(pcbnew.FromMM(0.12))
    gl.SetTextAngleDegrees(270)
    board.Add(gl)
    text("BAT+ 2S-4S", 27.5, EXT + 21.0, 0.8)
    text("GY-521: X arrow to TAIL", 33.5, 104.5, 0.9)
    return board, fps, nets


# ---------------------------------------------------------------------------
# Grid router
# ---------------------------------------------------------------------------
NX, NY = int(round(W / GRID)) + 1, int(round(H / GRID)) + 1
LAYERS = (pcbnew.F_Cu, pcbnew.B_Cu)


class Router:
    def __init__(self, board, nets):
        self.board = board
        self.nets = nets
        self.code = {n: i + 1 for i, n in enumerate(sorted(nets))}
        self.code.update({base(n): c for n, c in list(self.code.items())})
        xs = np.arange(NX) * GRID
        ys = np.arange(NY) * GRID
        self.X, self.Y = np.meshgrid(xs, ys, indexing="xy")      # [y, x]
        # copper shapes: (layer_index or None for both, kind, params, netcode)
        self.shapes = []
        for f in board.GetFootprints():
            for p in f.Pads():
                nc = self.code.get(p.GetNetname(), -1) if p.GetNetname() else -1
                both = p.GetAttribute() in (pcbnew.PAD_ATTRIB_PTH, pcbnew.PAD_ATTRIB_NPTH)
                bb = p.GetBoundingBox()
                x0, y0 = mm(bb.GetLeft()) - OX, mm(bb.GetTop()) - OY
                x1, y1 = mm(bb.GetRight()) - OX, mm(bb.GetBottom()) - OY
                if p.GetShape() == pcbnew.PAD_SHAPE_CIRCLE:
                    c = p.GetPosition()
                    self.add_shape(None if both else 0, "circle",
                                   (mm(c.x) - OX, mm(c.y) - OY, mm(p.GetSize().x) / 2), nc)
                else:
                    self.add_shape(None if both else 0, "rect", (x0, y0, x1, y1), nc)
        self.maps = {}
        self.holes = np.zeros((NY, NX), dtype=bool)
        for f in board.GetFootprints():
            for p in f.Pads():
                if p.GetAttribute() in (pcbnew.PAD_ATTRIB_PTH, pcbnew.PAD_ATTRIB_NPTH):
                    c = p.GetPosition()
                    self.add_hole(mm(c.x) - OX, mm(c.y) - OY, mm(p.GetDrillSize().x) / 2)

    def add_hole(self, x, y, r):
        """Cells where a via hole centre would be closer than 0.3 mm (edge to
        edge) to this hole."""
        lim = r + VIA_DRILL / 2 + 0.3
        i0, i1 = max(0, int((x - lim) / GRID) - 1), min(NX, int((x + lim) / GRID) + 2)
        j0, j1 = max(0, int((y - lim) / GRID) - 1), min(NY, int((y + lim) / GRID) + 2)
        sl = (slice(j0, j1), slice(i0, i1))
        self.holes[sl] |= np.hypot(self.X[sl] - x, self.Y[sl] - y) < lim

    def add_shape(self, layer, kind, prm, nc):
        self.shapes.append((layer, kind, prm, nc))
        for key, m in getattr(self, "maps", {}).items():
            self._stamp(m, key, layer, kind, prm, nc)

    def _dist(self, kind, prm, sl):
        X, Y = self.X[sl], self.Y[sl]
        if kind == "circle":
            cx, cy, r = prm
            return np.hypot(X - cx, Y - cy) - r
        if kind == "seg":
            ax, ay, bx, by, hw = prm
            dx, dy = bx - ax, by - ay
            L2 = dx * dx + dy * dy
            t = np.clip(((X - ax) * dx + (Y - ay) * dy) / L2, 0, 1) if L2 > 0 else 0
            return np.hypot(X - (ax + t * dx), Y - (ay + t * dy)) - hw
        x0, y0, x1, y1 = prm
        ddx = np.maximum(np.maximum(x0 - X, X - x1), 0)
        ddy = np.maximum(np.maximum(y0 - Y, Y - y1), 0)
        return np.hypot(ddx, ddy)

    def _bbox(self, kind, prm):
        if kind == "circle":
            cx, cy, r = prm
            return cx - r, cy - r, cx + r, cy + r
        if kind == "seg":
            ax, ay, bx, by, hw = prm
            return min(ax, bx) - hw, min(ay, by) - hw, max(ax, bx) + hw, max(ay, by) + hw
        return prm

    def _stamp(self, m, key, layer, kind, prm, nc):
        """Mark cells where a centreline of half-width key would violate clearance."""
        infl = key + CLEAR
        x0, y0, x1, y1 = self._bbox(kind, prm)
        i0, i1 = max(0, int((x0 - infl) / GRID) - 1), min(NX, int((x1 + infl) / GRID) + 2)
        j0, j1 = max(0, int((y0 - infl) / GRID) - 1), min(NY, int((y1 + infl) / GRID) + 2)
        if i0 >= i1 or j0 >= j1:
            return
        sl = (slice(j0, j1), slice(i0, i1))
        hit = self._dist(kind, prm, sl) < infl - 1e-6
        for li in ((0, 1) if layer is None else (layer,)):
            sub = m[li][sl]
            free = sub == 0
            sub[hit & free] = nc
            sub[hit & ~free & (sub != nc)] = -1

    def map_for(self, half_w):
        key = round(half_w, 3)
        if key not in self.maps:
            m = np.zeros((2, NY, NX), dtype=np.int32)
            # board edge keep-out
            e = int(math.ceil((EDGE_CLEAR + key) / GRID))
            m[:, :e, :] = -1
            m[:, -e:, :] = -1
            m[:, :, :e] = -1
            m[:, :, -e:] = -1
            for s in self.shapes:
                self._stamp(m, key, *s)
            self.maps[key] = m
        return self.maps[key]

    def route(self, a, b, nc, half_w, via_ok=True):
        """A* from grid node set a to set b. Nodes: (layer, j, i)."""
        m = self.map_for(half_w)
        vm = self.map_for(VIA_D / 2)
        goal = set(b)
        gj = np.mean([n[1] for n in b]); gi = np.mean([n[2] for n in b])
        def h(j, i):
            dj, di = abs(j - gj), abs(i - gi)
            return (max(dj, di) + 0.414 * min(dj, di))
        openq, g, prev = [], {}, {}
        for n in a:
            g[n] = 0.0
            heapq.heappush(openq, (h(n[1], n[2]), 0.0, n))
        dirs = [(0, 1, 1.0), (0, -1, 1.0), (1, 0, 1.0), (-1, 0, 1.0),
                (1, 1, 1.414), (1, -1, 1.414), (-1, 1, 1.414), (-1, -1, 1.414)]
        VIA_COST = 40.0
        LAYER_COST = (1.0, 1.15)          # prefer top for short hops
        expanded = 0
        while openq:
            f, gc, n = heapq.heappop(openq)
            if gc > g.get(n, 1e18):
                continue
            if n in goal:
                path = [n]
                while path[-1] in prev:
                    path.append(prev[path[-1]])
                return path[::-1]
            expanded += 1
            if expanded > 1_500_000:
                return None
            li, j, i = n
            for dj, di, c in dirs:
                jj, ii = j + dj, i + di
                if not (0 <= jj < NY and 0 <= ii < NX):
                    continue
                v = m[li, jj, ii]
                if v != 0 and v != nc:
                    continue
                if dj and di:      # no corner cutting
                    if m[li, j, ii] not in (0, nc) or m[li, jj, i] not in (0, nc):
                        continue
                nn = (li, jj, ii)
                ng = gc + c * LAYER_COST[li]
                if ng < g.get(nn, 1e18):
                    g[nn] = ng
                    prev[nn] = n
                    heapq.heappush(openq, (ng + h(jj, ii), ng, nn))
            if via_ok and not self.holes[j, i] and vm[0, j, i] in (0, nc) and vm[1, j, i] in (0, nc):
                nn = (1 - li, j, i)
                ng = gc + VIA_COST
                if ng < g.get(nn, 1e18):
                    g[nn] = ng
                    prev[nn] = n
                    heapq.heappush(openq, (ng + h(j, i), ng, nn))
        return None

    def commit(self, path, nc, width, netobj):
        """Turn a node path into tracks / vias and stamp them as obstacles."""
        pts = [(n[0], n[2] * GRID, n[1] * GRID) for n in path]
        segs, vias = [], []
        start = pts[0]
        for k in range(1, len(pts)):
            l0, x0, y0 = pts[k - 1]
            l1, x1, y1 = pts[k]
            if l1 != l0:
                if (x0, y0) != (start[1], start[2]):
                    segs.append((l0, start[1], start[2], x0, y0))
                vias.append((x0, y0))
                start = pts[k]
                continue
            if k + 1 < len(pts):
                l2, x2, y2 = pts[k + 1]
                if l2 == l1 and (round(x2 - x1, 4), round(y2 - y1, 4)) == (round(x1 - x0, 4), round(y1 - y0, 4)):
                    continue
            segs.append((l1, start[1], start[2], x1, y1))
            start = pts[k]
        for (l, ax, ay, bx, by) in segs:
            if (ax, ay) == (bx, by):
                continue
            t = pcbnew.PCB_TRACK(self.board)
            t.SetStart(pt(ax, ay))
            t.SetEnd(pt(bx, by))
            t.SetWidth(pcbnew.FromMM(width))
            t.SetLayer(LAYERS[l])
            t.SetNet(netobj)
            self.board.Add(t)
            self.add_shape(l, "seg", (ax, ay, bx, by, width / 2), nc)
        for (x, y) in vias:
            v = pcbnew.PCB_VIA(self.board)
            v.SetPosition(pt(x, y))
            v.SetWidth(pcbnew.FromMM(VIA_D))
            v.SetDrill(pcbnew.FromMM(VIA_DRILL))
            v.SetNet(netobj)
            self.board.Add(v)
            self.add_shape(None, "circle", (x, y, VIA_D / 2), nc)
            self.add_hole(x, y, VIA_DRILL / 2)
        return len(segs), len(vias)


def pad_nodes(pad, half_w):
    """Grid nodes usable to start / end on a pad (cells near its centre)."""
    c = pad.GetPosition()
    x, y = mm(c.x) - OX, mm(c.y) - OY
    i, j = int(round(x / GRID)), int(round(y / GRID))
    both = pad.GetAttribute() == pcbnew.PAD_ATTRIB_PTH
    nodes = []
    for dj in range(-2, 3):
        for di in range(-2, 3):
            for li in ((0, 1) if both else (0,)):
                nodes.append((li, j + dj, i + di))
    return nodes


def route_all(board, nets):
    r = Router(board, nets)
    pads_by_net = {}
    for f in board.GetFootprints():
        for p in f.Pads():
            if p.GetNetname() and not p.GetNetname().startswith("unconnected-"):
                pads_by_net.setdefault(p.GetNetname(), []).append(p)

    order = sorted((n for n in pads_by_net if base(n) != "GND"),
                   key=lambda n: (base(n) not in WIDTH, len(pads_by_net[n])))
    failed = []
    for name in order:
        pads = pads_by_net[name]
        if len(pads) < 2:
            continue
        nc = r.code[name]
        w = WIDTH.get(base(name), SIG_W)
        tree = list(pad_nodes(pads[0], w / 2))
        todo = pads[1:]
        while todo:
            # nearest remaining pad to the tree start (cheap heuristic)
            tj = np.mean([n[1] for n in tree]); ti = np.mean([n[2] for n in tree])
            todo.sort(key=lambda p: math.hypot(mm(p.GetPosition().x) - OX - ti * GRID,
                                               mm(p.GetPosition().y) - OY - tj * GRID))
            p = todo.pop(0)
            path = r.route(tree, pad_nodes(p, w / 2), nc, w / 2)
            if path is None:
                failed.append((name, p.GetParentFootprint().GetReference() + "." + p.GetNumber()))
                print(f"  FAIL {name} -> {failed[-1][1]}")
                continue
            ns, nv = r.commit(path, nc, w, nets[name])
            tree.extend(path)
            print(f"  {name:12s} -> {p.GetParentFootprint().GetReference()}.{p.GetNumber():3s} {ns} segs {nv} vias")
    return r, failed


def log(*a):
    print(*a, flush=True)


def pours(board, nets, router):
    gnd = nets["/GND"] if "/GND" in nets else nets["GND"]
    for layer in LAYERS:
        log("zone", layer)
        z = pcbnew.ZONE(board)
        z.SetLayer(layer)
        z.SetNet(gnd)
        z.SetLocalClearance(pcbnew.FromMM(0.3))
        z.SetMinThickness(pcbnew.FromMM(0.25))
        z.SetPadConnection(pcbnew.ZONE_CONNECTION_THERMAL)
        z.SetThermalReliefGap(pcbnew.FromMM(0.3))
        z.SetThermalReliefSpokeWidth(pcbnew.FromMM(0.4))
        z.SetIslandRemovalMode(pcbnew.ISLAND_REMOVAL_MODE_ALWAYS)
        log(" outline")
        poly = pcbnew.SHAPE_LINE_CHAIN()
        for (x, y) in ((0.3, 0.3), (W - 0.3, 0.3), (W - 0.3, H - 0.3), (0.3, H - 0.3)):
            poly.Append(pcbnew.FromMM(OX + x), pcbnew.FromMM(OY + y))
        poly.SetClosed(True)
        z.Outline().AddOutline(poly)
        board.Add(z)
        log(" added")
    # One via next to every SMD GND pad (short return path, and no SMD
    # ground can end up on an isolated piece of the top pour)
    nc0 = router.code[gnd.GetNetname()]
    vm0 = router.map_for(VIA_D / 2)
    tm = router.map_for(0.2)
    pv = 0
    for f in board.GetFootprints():
        for p in f.Pads():
            if p.GetNetname() != gnd.GetNetname() or p.GetAttribute() != pcbnew.PAD_ATTRIB_SMD:
                continue
            c = p.GetPosition()
            px, py = mm(c.x) - OX, mm(c.y) - OY
            pi, pj = int(round(px / GRID)), int(round(py / GRID))
            best = None
            for rad in range(6, 30):                       # 0.76 .. 3.8 mm
                for k in range(0, 360, 15):
                    i = pi + int(round(rad * math.cos(math.radians(k))))
                    j = pj + int(round(rad * math.sin(math.radians(k))))
                    if not (0 <= i < NX and 0 <= j < NY):
                        continue
                    if vm0[0, j, i] != 0 or vm0[1, j, i] != 0 or router.holes[j, i]:
                        continue
                    steps = max(abs(i - pi), abs(j - pj))
                    ok = all(tm[0, pj + round((j - pj) * t / steps), pi + round((i - pi) * t / steps)] in (0, nc0)
                             for t in range(steps + 1))
                    if ok:
                        best = (i, j)
                        break
                if best:
                    break
            if not best:
                print("  no GND via spot near", f.GetReference(), p.GetNumber())
                continue
            vx, vy = best[0] * GRID, best[1] * GRID
            t = pcbnew.PCB_TRACK(board)
            t.SetStart(c)
            t.SetEnd(pt(vx, vy))
            t.SetWidth(pcbnew.FromMM(0.4))
            t.SetLayer(pcbnew.F_Cu)
            t.SetNet(gnd)
            board.Add(t)
            v = pcbnew.PCB_VIA(board)
            v.SetPosition(pt(vx, vy))
            v.SetWidth(pcbnew.FromMM(VIA_D))
            v.SetDrill(pcbnew.FromMM(VIA_DRILL))
            v.SetNet(gnd)
            board.Add(v)
            router.add_shape(0, "seg", (px, py, vx, vy, 0.2), nc0)
            router.add_shape(None, "circle", (vx, vy, VIA_D / 2), nc0)
            router.add_hole(vx, vy, VIA_DRILL / 2)
            pv += 1
    log("GND pad vias:", pv)

    # stitching vias where both layers are free for a via
    vm = router.map_for(VIA_D / 2)
    nc = router.code[gnd.GetNetname()]
    step = int(round(5.08 / GRID))
    n = 0
    for j in range(step // 2, NY, step):
        for i in range(step // 2, NX, step):
            if vm[0, j, i] == 0 and vm[1, j, i] == 0 and not router.holes[j, i]:
                v = pcbnew.PCB_VIA(board)
                v.SetPosition(pt(i * GRID, j * GRID))
                v.SetWidth(pcbnew.FromMM(VIA_D))
                v.SetDrill(pcbnew.FromMM(VIA_DRILL))
                v.SetNet(gnd)
                board.Add(v)
                router.add_shape(None, "circle", (i * GRID, j * GRID, VIA_D / 2), nc)
                router.add_hole(i * GRID, j * GRID, VIA_DRILL / 2)
                n += 1
    log("stitch vias", n, "(zones are filled by fill_zones.py: ZONE_FILLER crashes on a board without a project)")
    return n


def main():
    board, fps, nets = build()
    print("placed", len(fps), "footprints;", len(nets), "nets")
    router, failed = route_all(board, nets)
    board.Save(OUT)
    log("saved routed board (before pours)")
    n = pours(board, nets, router)
    print("stitching vias:", n)
    board.Save(OUT)
    print("saved", OUT, "| unrouted:", failed if failed else "none")


if __name__ == "__main__":
    main()
