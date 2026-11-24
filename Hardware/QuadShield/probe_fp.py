"""Print pad offsets and courtyard size of every footprint used (rotation 0)."""
import pcbnew
import design

FP = "C:/Users/Lenovo/AppData/Local/Programs/KiCad/8.0/share/kicad/footprints"
seen = set()
for ref, (val, sym, fp, pins, lcsc, note) in design.PARTS.items():
    if fp in seen:
        continue
    seen.add(fp)
    lib, name = fp.split(":")
    f = pcbnew.FootprintLoad(f"{FP}/{lib}.pretty", name)
    pads = sorted(f.Pads(), key=lambda p: (len(p.GetNumber()), p.GetNumber()))
    cy = f.GetCourtyard(pcbnew.F_CrtYd).BBox()
    def mm(v):
        return round(pcbnew.ToMM(v), 2)
    pl = [(p.GetNumber(), mm(p.GetPosition().x), mm(p.GetPosition().y), mm(p.GetSize().x), mm(p.GetSize().y),
           "THT" if p.GetAttribute() == pcbnew.PAD_ATTRIB_PTH else "SMD") for p in pads[:4]]
    print(f"{ref:4s} {name:52s} crt x[{mm(cy.GetLeft())},{mm(cy.GetRight())}] y[{mm(cy.GetTop())},{mm(cy.GetBottom())}] pads={len(pads)} {pl}")
