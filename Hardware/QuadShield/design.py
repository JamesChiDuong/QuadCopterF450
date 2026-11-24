"""QuadShield - design data (single source of truth for schematic + PCB).

Shield PCB that sits UNDER the STM32F429I-DISC1 (MB1075) board: two 2x32
female sockets receive the DISC1 header pins P1 / P2. All pin numbers and
mechanics are from UM1670 Rev 5 (Documents/um1670-...pdf):
  - board 66.00 x 119.30 mm, headers 2.54 mm pitch, 32 rows
  - P1 odd pins (1,3..63) on the OUTER left column x = 6.33 mm,
    P1 even pins on x = 8.87 mm; P2 odd pins on the INNER column
    x = 57.13 mm, P2 even pins on x = 59.67 mm (top view, CN1 USB at top)
  - row 1 at y = 33.82 mm from the top edge, last row at 112.56 mm
  - CN1 (ST-LINK USB) at the middle of the top edge, CN6 (USB OTG, gamepad)
    at the middle of the bottom edge: keep tall parts away from x = 23..43.

Signals used by the firmware (Core/Inc/USER_Config.h, USER_GPS.c):
  ESC M1 PB4 (P1-25)  M2 PA8 (P1-53)  M3 PB0 (P2-28)  M4 PB10 (P2-48)
  MPU6050 I2C2: SDA PF0 (P1-7), SCL PF1 (P1-8)
  GPS UART5: MCU TX PC12 (P1-43) -> GPS RX, MCU RX PD2 (P1-40) <- GPS TX
  RC: PE2 (P1-15) PE3 (P1-16) PE4 (P1-13) PE5 (P1-14) PE6 (P1-11)
  Battery sense ADC: PC3 (P2-15, ADC123_IN13)   Buzzer: PD4 (P1-38)
  Power: 5V into P1-1 / P1-2 (UM1670 6.4: 5 V pins may be used as input),
         3V (P2-1 / P2-2) used as 3.3 V supply for the MPU (output, <100 mA)
         GND P1-63/64, P2-11/29/63

Power: XT30 battery 2S..4S -> 1.5 A polyfuse -> SS34 reverse protection ->
AP63205 (5.0 V fixed, 2 A, Diodes DS41326: FB=VOUT, EN=VIN, 4.7 uH,
Cin 10 uF, Cout 2 x 22 uF, Cbst 100 nF) -> SS34 OR-diode -> DISC1 5V.
The OR diode stops the buck from back-feeding the PC through USB and lets
the USB power the board when no battery is connected.
The ESC red (+5 V BEC) wires are NOT connected (middle pin of the ESC
headers is left open) - that wire caused the MCU resets before.
"""

# ---------------------------------------------------------------------------
# DISC1 header map (UM1670 table 7). None = NC / not needed.
# ---------------------------------------------------------------------------
P1 = {1: "5V", 2: "5V", 3: "PF4", 4: "PF5", 5: "PF2", 6: "PF3", 7: "PF0", 8: "PF1",
      9: "PC14", 10: "PC15", 11: "PE6", 12: "PC13", 13: "PE4", 14: "PE5", 15: "PE2",
      16: "PE3", 17: "PE0", 18: "PE1", 19: "PB8", 20: "PB9", 21: "BOOT0", 22: "VDD",
      23: "PB6", 24: "PB7", 25: "PB4", 26: "PB5", 27: "PG15", 28: "PB3", 29: "PG13",
      30: "PG14", 31: "PG11", 32: "PG12", 33: "PG9", 34: "PG10", 35: "PD7", 36: "PD6",
      37: "PD5", 38: "PD4", 39: "PD3", 40: "PD2", 41: "PD1", 42: "PD0", 43: "PC12",
      44: "PC11", 45: "PC10", 46: "PA15", 47: "PA14", 48: "PA13", 49: "PA12",
      50: "PA11", 51: "PA10", 52: "PA9", 53: "PA8", 54: "PC9", 55: "PC8", 56: "PC7",
      57: "PC6", 58: "PG8", 59: "PG7", 60: "PG6", 61: "PG5", 62: "PG4", 63: "GND",
      64: "GND"}
P2 = {1: "3V", 2: "3V", 3: "PF6", 4: None, 5: "PF8", 6: "PF7", 7: "PF10", 8: "PF9",
      9: "PH1", 10: "PH0", 11: "GND", 12: "NRST", 13: "PC1", 14: "PC0", 15: "PC3",
      16: "PC2", 17: "PA1", 18: "PA0", 19: "PA3", 20: "PA2", 21: "PA5", 22: "PA4",
      23: "PA7", 24: "PA6", 25: "PC5", 26: "PC4", 27: "PB1", 28: "PB0", 29: "GND",
      30: "PB2", 31: "PF12", 32: "PF11", 33: "PF14", 34: "PF13", 35: "PG0", 36: "PF15",
      37: "PE7", 38: "PG1", 39: "PE9", 40: "PE8", 41: "PE11", 42: "PE10", 43: "PE13",
      44: "PE12", 45: "PE15", 46: "PE14", 47: "PB11", 48: "PB10", 49: "PB13",
      50: "PB12", 51: "PB15", 52: "PB14", 53: "PD9", 54: "PD8", 55: "PD11",
      56: "PD10", 57: "PD13", 58: "PD12", 59: "PD15", 60: "PD14", 61: "PG3",
      62: "PG2", 63: "GND", 64: None}

# MCU pin name -> shield net name (only the pins the shield uses)
MCU_NETS = {
    "5V": "+5V", "3V": "+3V3", "GND": "GND",
    "PB4": "ESC1", "PA8": "ESC2", "PB0": "ESC3", "PB10": "ESC4",
    "PF0": "I2C_SDA", "PF1": "I2C_SCL",
    "PC12": "GPS_RX", "PD2": "GPS_TX",          # names from the GPS side
    "PE2": "RC1", "PE3": "RC2", "PE4": "RC3", "PE5": "RC4", "PE6": "RC5",
    "PC3": "VBAT_SENSE", "PD4": "BUZZER",
}

# ---------------------------------------------------------------------------
# Components: ref -> (value, KiCad symbol lib:name, footprint lib:name,
#                     {pin: net}, LCSC part (only filled when verified), note)
# ---------------------------------------------------------------------------
def header_pins(pmap):
    return {n: MCU_NETS.get(sig) for n, sig in pmap.items() if MCU_NETS.get(sig)}

PARTS = {
    # DISC1 sockets (2x32 female, 8.5 mm high)
    "J1": ("DISC1 P1", "Connector_Generic:Conn_02x32_Odd_Even",
           "Connector_PinHeader_2.54mm:PinHeader_2x32_P2.54mm_Vertical",
           header_pins(P1), "", "2x32 FEMALE socket; PinHeader pad pattern on purpose (KiCad PinSocket is mirrored: pin2 at -x)"),
    "J2": ("DISC1 P2", "Connector_Generic:Conn_02x32_Odd_Even",
           "Connector_PinHeader_2.54mm:PinHeader_2x32_P2.54mm_Vertical",
           header_pins(P2), "", "2x32 FEMALE socket; PinHeader pad pattern on purpose (KiCad PinSocket is mirrored: pin2 at -x)"),

    # Battery input + protection + 5 V buck
    "J3": ("XT30 battery", "Connector_Generic:Conn_01x02",
           "Connector_AMASS:AMASS_XT30PW-M_1x02_P2.50mm_Horizontal",
           {1: "VBAT_RAW", 2: "GND"}, "", "2S-4S LiPo, pin 1 = +"),
    "F1": ("1.5A PTC", "Device:Polyfuse", "Fuse:Fuse_1812_4532Metric",
           {1: "VBAT_RAW", 2: "VBAT_F"}, "", "resettable fuse"),
    "D1": ("SS34", "Device:D_Schottky", "Diode_SMD:D_SMA",
           {1: "VBAT", 2: "VBAT_F"}, "", "reverse polarity (K=1)"),
    "C1": ("10uF 35V", "Device:C", "Capacitor_SMD:C_1206_3216Metric",
           {1: "VBAT", 2: "GND"}, "", "buck input"),
    "C2": ("100nF 50V", "Device:C", "Capacitor_SMD:C_0603_1608Metric",
           {1: "VBAT", 2: "GND"}, "", "buck input HF"),
    "U1": ("AP63205WU-7", "Regulator_Switching:AP63205WU",
           "Package_TO_SOT_SMD:TSOT-23-6",
           {1: "+5V_BUCK", 2: "VBAT", 3: "VBAT", 4: "GND", 5: "SW", 6: "BST"},
           "C2071056", "FB=VOUT, EN=VIN (DS41326 fig.1)"),
    "C3": ("100nF", "Device:C", "Capacitor_SMD:C_0603_1608Metric",
           {1: "BST", 2: "SW"}, "", "bootstrap"),
    "L1": ("4.7uH 3A", "Device:L", "Inductor_SMD:L_Taiyo-Yuden_NR-50xx",
           {1: "SW", 2: "+5V_BUCK"}, "", "shielded, Isat >= 3 A"),
    "C4": ("22uF 10V", "Device:C", "Capacitor_SMD:C_1206_3216Metric",
           {1: "+5V_BUCK", 2: "GND"}, "", "buck output"),
    "C5": ("22uF 10V", "Device:C", "Capacitor_SMD:C_1206_3216Metric",
           {1: "+5V_BUCK", 2: "GND"}, "", "buck output"),
    "D2": ("SS34", "Device:D_Schottky", "Diode_SMD:D_SMA",
           {1: "+5V", 2: "+5V_BUCK"}, "", "OR diode -> DISC1 5V"),
    "C6": ("10uF 10V", "Device:C", "Capacitor_SMD:C_0805_2012Metric",
           {1: "+5V", 2: "GND"}, "", "5V rail"),
    "R1": ("2k2", "Device:R", "Resistor_SMD:R_0603_1608Metric",
           {1: "+5V_BUCK", 2: "LED_PWR"}, "", "power LED"),
    "D3": ("LED green", "Device:LED", "LED_SMD:LED_0603_1608Metric",
           {1: "GND", 2: "LED_PWR"}, "", "K=1, A=2"),

    # Battery voltage sense: 100k / 22k -> 4S 16.8 V = 3.03 V max at the ADC
    "R2": ("100k", "Device:R", "Resistor_SMD:R_0603_1608Metric",
           {1: "VBAT", 2: "VBAT_SENSE"}, "", "divider top"),
    "R3": ("22k", "Device:R", "Resistor_SMD:R_0603_1608Metric",
           {1: "VBAT_SENSE", 2: "GND"}, "", "divider bottom"),
    "C7": ("100nF", "Device:C", "Capacitor_SMD:C_0603_1608Metric",
           {1: "VBAT_SENSE", 2: "GND"}, "", "ADC filter"),

    # Buzzer: active 5 V buzzer, low-side NPN
    "BZ1": ("Buzzer 5V", "Device:Buzzer", "Buzzer_Beeper:Buzzer_12x9.5RM7.6",
            {1: "+5V", 2: "BZ_N"}, "", "active buzzer, + = pin 1"),
    "Q1": ("MMBT3904", "Transistor_BJT:MMBT3904", "Package_TO_SOT_SMD:SOT-23",
           {1: "BZ_B", 2: "GND", 3: "BZ_N"}, "", "B=1 E=2 C=3"),
    "R4": ("1k", "Device:R", "Resistor_SMD:R_0603_1608Metric",
           {1: "BUZZER", 2: "BZ_B"}, "", "base"),
    "R5": ("10k", "Device:R", "Resistor_SMD:R_0603_1608Metric",
           {1: "BZ_B", 2: "GND"}, "", "keeps it off at reset"),
    "D4": ("1N4148W", "Device:D", "Diode_SMD:D_SOD-123",
           {1: "+5V", 2: "BZ_N"}, "", "flyback, K=1"),

    # ESC outputs: signal / NC (red BEC wire) / GND
    "J4": ("ESC M1", "Connector_Generic:Conn_01x03",
           "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical",
           {1: "ESC1", 3: "GND"}, "", "M1 front-left, middle pin open"),
    "J5": ("ESC M2", "Connector_Generic:Conn_01x03",
           "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical",
           {1: "ESC2", 3: "GND"}, "", "M2 back-left"),
    "J6": ("ESC M3", "Connector_Generic:Conn_01x03",
           "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical",
           {1: "ESC3", 3: "GND"}, "", "M3 front-right"),
    "J7": ("ESC M4", "Connector_Generic:Conn_01x03",
           "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical",
           {1: "ESC4", 3: "GND"}, "", "M4 back-right"),

    # GY-521 (MPU6050) module soldered flat, pins: VCC GND SCL SDA XDA XCL AD0 INT
    "J8": ("GY-521", "Connector_Generic:Conn_01x08",
           "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical",
           {1: "+3V3", 2: "GND", 3: "I2C_SCL", 4: "I2C_SDA", 7: "GND"}, "",
           "AD0 = GND -> address 0x68; X arrow to the TAIL"),

    # GPS (u-blox M8N style): 5V, TX, RX, SCL, SDA, GND
    "J9": ("GPS", "Connector_Generic:Conn_01x06",
           "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical",
           {1: "+5V", 2: "GPS_TX", 3: "GPS_RX", 4: "I2C_SCL", 5: "I2C_SDA", 6: "GND"},
           "", "GPS TX -> PD2, GPS RX <- PC12, compass on I2C2"),

    # RC receiver: 5 channels, each S / +5V / GND (servo layout)
    **{f"J{10 + i}": (f"RC CH{i + 1}", "Connector_Generic:Conn_01x03",
                      "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical",
                      {1: f"RC{i + 1}", 2: "+5V", 3: "GND"}, "",
                      "receiver channel (RC FULL build)") for i in range(5)},

    # I2C pull-ups (fit only if neither the GY-521 nor the GPS has them)
    "R6": ("4k7 DNP", "Device:R", "Resistor_SMD:R_0603_1608Metric",
           {1: "+3V3", 2: "I2C_SCL"}, "", "optional pull-up"),
    "R7": ("4k7 DNP", "Device:R", "Resistor_SMD:R_0603_1608Metric",
           {1: "+3V3", 2: "I2C_SDA"}, "", "optional pull-up"),

    # Mounting holes (positions decided later with the F450 plate)
}

BOARD = dict(width=66.0, disc1_len=119.30, ext=28.0)   # extension below DISC1 for tall parts
