"""Refill all copper zones of QuadShield.kicad_pcb (run with KiCad's Python).

ZONE_FILLER crashes on a board made with CreateEmptyBoard() (no project),
so gen_pcb.py saves first and this script loads the file and fills."""
import sys
import pcbnew

path = sys.argv[1] if len(sys.argv) > 1 else "QuadShield.kicad_pcb"
board = pcbnew.LoadBoard(path)
print("loaded", path, "zones:", len(board.Zones()), flush=True)
pcbnew.ZONE_FILLER(board).Fill(board.Zones())
print("filled", flush=True)
board.Save(path)
print("saved", flush=True)
