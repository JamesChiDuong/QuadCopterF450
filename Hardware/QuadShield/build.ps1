# build.ps1 - regenerate the whole QuadShield design from design.py
#   1. schematic (gen_sch.py)          2. netlist (kicad-cli)
#   3. PCB placement + routing (gen_pcb.py, KiCad Python)
#   4. zone fill (fill_zones.py)        5. ERC + DRC reports (erc.rpt, drc.rpt)
#   6. with -Fab: Gerbers, drill, BOM, positions, PDF in fab/
# Run from this folder:  powershell -ExecutionPolicy Bypass -File build.ps1 [-Fab]
param([switch]$Fab)
$ErrorActionPreference = "Stop"
$kicad = "$env:LOCALAPPDATA\Programs\KiCad\8.0\bin"
$cli = "$kicad\kicad-cli.exe"
$kpy = "$kicad\python.exe"
Set-Location $PSScriptRoot

function Run-Py($script) {
    $p = Start-Process -FilePath $kpy -ArgumentList "-u", $script -NoNewWindow -Wait -PassThru `
        -RedirectStandardOutput "$script.log" -RedirectStandardError "$script.err"
    Get-Content "$script.log" | Select-Object -Last 4
    if ($p.ExitCode -ne 0) { Get-Content "$script.err" | Select-Object -Last 10; throw "$script failed ($($p.ExitCode))" }
}

Run-Py gen_sch.py
& $cli sch export netlist --format kicadsexpr -o QuadShield.net QuadShield.kicad_sch | Out-Null
Run-Py gen_pcb.py
Run-Py fill_zones.py
& $cli sch erc --severity-all -o erc.rpt QuadShield.kicad_sch | Select-Object -Last 1
& $cli pcb drc --schematic-parity --severity-all -o drc.rpt QuadShield.kicad_pcb | Select-Object -Last 3

if ($Fab) {
    New-Item -ItemType Directory -Force fab | Out-Null
    & $cli pcb export gerbers --layers "F.Cu,B.Cu,F.Paste,B.Paste,F.SilkS,B.SilkS,F.Mask,B.Mask,Edge.Cuts" -o fab/ QuadShield.kicad_pcb | Out-Null
    & $cli pcb export drill --format excellon --excellon-separate-th -o fab/ QuadShield.kicad_pcb | Out-Null
    & $cli pcb export pos --side front --format csv --units mm --exclude-dnp -o fab/QuadShield-pos.csv QuadShield.kicad_pcb | Out-Null
    & $cli sch export bom --fields "Reference,Value,Footprint,LCSC,Description,`${QUANTITY},`${DNP}" --group-by "Value,Footprint" -o fab/QuadShield-bom.csv QuadShield.kicad_sch | Out-Null
    & $cli sch export pdf -o fab/QuadShield-schematic.pdf QuadShield.kicad_sch | Out-Null
    & $cli pcb export pdf --layers "F.Cu,F.SilkS,Edge.Cuts" -o fab/QuadShield-top.pdf QuadShield.kicad_pcb | Out-Null
    & $cli pcb export pdf --layers "B.Cu,B.SilkS,Edge.Cuts" --mirror -o fab/QuadShield-bottom.pdf QuadShield.kicad_pcb | Out-Null
    Compress-Archive -Force -Path fab/*.gbr, fab/*.g*, fab/*.drl -DestinationPath fab/QuadShield-gerbers.zip
    Get-ChildItem fab | Select-Object Name, Length
}
