# QuadCopter F450 – STM32F429 Flight Controller

Firmware điều khiển quadcopter F450 trên STM32F429I-DISC1 + FreeRTOS: đọc MPU6050, ước lượng góc, PID cascade, trộn 4 motor (X), đọc tay điều khiển RC, arm/disarm và failsafe. Có lệnh UART để test motor, tune PID và xem telemetry.

> ⚠️ **AN TOÀN:** Luôn **tháo cánh quạt** khi test trên bàn, khi đổi cấu hình hoặc nạp firmware mới. Chỉ lắp cánh sau khi đã qua hết checklist ở mục 5.

---

## 0. Chọn phiên bản build

Trong `Core/Inc/USER_Config.h`:

```c
#define FC_VARIANT   FC_VARIANT_RC_FULL     // hoặc FC_VARIANT_UART_TEST, FC_VARIANT_GAMEPAD
```

| | `FC_VARIANT_RC_FULL` (1) | `FC_VARIANT_UART_TEST` (2) | `FC_VARIANT_GAMEPAD` (3) |
|---|---|---|---|
| Thiết bị điều khiển | Tay RC + receiver, RX nối PE2–PE6 | Không có, chỉ lệnh UART | **Tay cầm USB** (GameSir Nova 2 Lite ở chế độ Switch hoặc Xbox…) cắm vào **CN6** |
| Arm | Công tắc CH5 | Lệnh `@QC+ARM` | **Giữ START** 0.5 s |
| Disarm | Gạt CH5 | `@QC+DISARM` | **B** hoặc **BACK** (cả `@QC+DISARM`) |
| Điều khiển | Cần gạt RC | `@QC+SP=...` / bàn phím qua `drone_viewer.py --control` | Cần tay cầm (mục 5, bước 7c) |
| Giới hạn ga | 1850 µs | 1300 µs (chỉ test trên bàn) | 1850 µs |
| Failsafe | Mất sóng RC > 100 ms | Tùy chọn `UART_SP_TIMEOUT_MS` | **Rút / tắt tay cầm** (mất kết nối USB) |
| Màn hình | Các kênh RC | Setpoint UART | Loại tay cầm, trạng thái và vị trí cần |
| Chân dùng thêm | PE2–PE6 | — | PB13/PB14/PB15 (USB), **PC4** (bật nguồn USB) |
| Clock | HSI | HSI | **HSE 8 MHz** (USB cần clock chính xác) |

Muốn giữ nhiều phiên bản mà không phải sửa file: trong CubeIDE tạo thêm Build Configuration, rồi vào *Properties → C/C++ Build → Settings → MCU GCC Compiler → Preprocessor* thêm `FC_VARIANT=2` (hoặc `3`). Dòng lệnh: `make VARIANT=3`. Tương tự, `UI_LCD_ENABLE=0` tắt màn hình.

> ⚠️ **Bản GAMEPAD dùng PC4 để bật nguồn 5 V cho cổng USB CN6** (theo thiết kế của board DISC1). Chân này trước đây là `BLDC3_VCC`. Nếu có dây nào đang cắm vào PC4 thì **rút ra** trước khi nạp bản GAMEPAD.

---

## 1. Phần cứng

| Thành phần | Ghi chú |
|---|---|
| Khung F450, 4× motor A2212/15T 930KV | |
| 4× ESC SimonK 30A | Nhận xung 1000–2000 µs, PWM 400 Hz |
| STM32F429I-DISC1 | SYSCLK 72 MHz (HSI) |
| MPU6050 | I2C2 (100 kHz mặc định, `MPU6050_I2C_SPEED_HZ`) |
| RX 6 kênh (PWM) | 5 kênh được dùng |
| LiPo 3S 2200 mAh, BEC 5V 3A | |

### Sơ đồ chân và đấu nối

Bảng dưới là **đúng những gì firmware đang cấu hình** (đã kiểm tra trong `main.h`, `stm32f4xx_hal_msp.c`, `USER_Motor_Driver.c`, `usbh_conf.c`).

#### a) Cảm biến MPU6050 (module GY-521), I2C2

**Hướng lắp hiện tại:** module nằm ngửa (linh kiện hướng lên), **mũi tên X in trên module chỉ về đuôi máy** (phía M2/M4), trục Y của chip chỉ sang phải (đã đo bằng bài test nghiêng ngày 22/11/2026). Firmware quy đổi: `x_thân = −X_chip, y_thân = +Y_chip, z_thân = −Z_chip` (`IMU_BODY_X/Y/Z` trong `USER_Config.h`; trong file có sẵn công thức cho các hướng lắp khác). Kiểm tra bằng `@QC+DEBUG=1`: ngóc mũi (M1, M3) lên thì **pitch dương**, hạ bên phải (M3, M4) xuống thì **roll dương**, xoay mũi sang phải thì **yaw tăng**.

| Chân module | Chân board | Firmware cấu hình |
|---|---|---|
| VCC | **PF5** | GPIO xuất HIGH (3.3 V) cấp nguồn cho module (hoặc nối thẳng vào chân 3V của board) |
| GND | **PF4** | GPIO xuất LOW (hoặc nối thẳng vào GND của board) |
| SCL | **PF1** | I2C2_SCL, AF4, open-drain, 100 kHz |
| SDA | **PF0** | I2C2_SDA, AF4, open-drain |
| AD0 | để trống / GND | địa chỉ 0x68 (nối VCC thì 0x69, firmware tự dò) |
| INT, XDA, XCL | không nối | — |

```
   GY-521                      STM32F429I-DISC1
   VCC ───────────────────────► PF5   (HIGH)
   GND ───────────────────────► PF4   (LOW)
   SCL ───────────────────────► PF1   (I2C2_SCL)
   SDA ───────────────────────► PF0   (I2C2_SDA)
```

Dây I2C nên ngắn (dưới khoảng 20–30 cm) và tránh đi song song với dây motor.

#### b) 4 ESC → board (cáp servo 3 sợi của mỗi ESC)

| Motor | Vị trí / chiều quay | Giắc servo cắm vào header | Dây **tín hiệu** (trắng/vàng) | Dây **đỏ +5 V** (chân giữa, firmware để **Hi-Z**) | Dây **GND** (đen/nâu) → chân xuất **LOW** |
|---|---|---|---|---|---|
| M1 | trước-trái, **CW** | P1-25 / 27 / 29 | **PB4** (TIM3 CH1) | PG15 | **PG13** |
| M2 | sau-trái, **CCW** | P1-53 / 55 / 57 | **PA8** (TIM1 CH1) | PC8 | **PC6** |
| M3 | trước-phải, **CCW** | **P2-28 / 30 / 32** (đã dời) | **PB0** (TIM3 CH3) | **PB2** | **PF11** |
| M4 | sau-phải, **CW** | P2-48 / 46 / 44 | **PB10** (TIM2 CH3) | PE14 | **PE12** |

Giắc servo 3 chân cắm vào **3 chân cùng hàng liên tiếp** của header (số chân n, n+2, n+4). **M3 đã được dời** từ P2-24/26/28 (PA6 / PC4 / PB0) sang **P2-28/30/32 (PB0 / PB2 / PF11)**: giắc dịch sang một nấc và **xoay ngược 180°** (tín hiệu ở P2-28, GND ở P2-32), vì PC4 là chân bật nguồn USB ở bản GAMEPAD và luôn bị giữ LOW, nên sẽ chập dây đỏ 5 V của ESC. PB2 chịu được 5 V (FT). Muốn đổi chân, sửa bảng `ESCx_GND_*` / `ESCx_VCC_*` trong `USER_Config.h`.

```
   ESC (cáp servo)                 STM32F429I-DISC1
   trắng/vàng (tín hiệu) ───────► PB4 / PA8 / PB0 / PB10
   đỏ        (+5 V BEC)  ───────► chân giữa của giắc (PG15 / PC8 / PB2 / PE14), firmware để Hi-Z
   đen/nâu   (GND)       ───────► chân LOW (PG13 / PC6 / PF11 / PE12)  hoặc GND của board
```

> ⚠️ **Ba điều phải đúng khi nối ESC:**
> 1. **Dây GND của ESC phải vào chân xuất LOW** (cột "GND" ở bảng trên) **hoặc một chân GND thật của board**, không bao giờ vào chân xuất HIGH. Ví dụ với M1: GND vào **PG13**, không phải PG15 (PG15 đang được firmware kéo lên 3.3 V). Muốn đổi chân, sửa code trước rồi mới nối dây.
> 2. **Dây đỏ +5 V của ESC là nguồn do ESC phát ra** (khi cắm pin), không phải đầu vào. Nếu tách được dây thì để trống là tốt nhất. Với giắc 3 chân liền khối, dây đỏ nằm trên chân giữa: chân đó **phải là chân chịu được 5 V (FT) và được firmware để ở Hi-Z**. Bảng trên đã đảm bảo điều này; **tuyệt đối không** đặt chân giữa lên một chân đang xuất HIGH hay LOW (ví dụ PC4 ở bản GAMEPAD).
> 3. Các chân "VCC" cũ (PG15, PC8, PC4, PE14) **đã được tắt trong firmware** (chuyển sang input trở kháng cao, `MOTOR_ESC_VCC_PINS_HIGH = 0`). Trước đây chúng bị kéo HIGH: nếu dây đỏ 5 V của ESC cắm vào đó thì khi cắm pin, 5 V đấu với 3.3 V và **MCU bị reset**. Không cần nối gì vào các chân này.
>
> Khi khởi động, firmware in `> Reset cause: ...` ra UART. Nếu thấy `BOR(brown-out...)` sau khi cắm pin, nghĩa là nguồn 3.3 V bị sụt: kiểm tra lại dây GND và dây đỏ của ESC.
>
> Thiếu chân GND trên board thì gom 4 dây GND của ESC lại (domino hoặc breadboard) rồi nối vào **một** chân GND của header P1/P2. Cách này chắc chắn hơn dùng GPIO làm GND.

Phía động lực của ESC: 2 dây to đỏ/đen vào pin 3S (qua tấm phân phối nguồn của khung F450 / giắc XT60), 3 dây pha vào motor. Thứ tự 3 dây pha không quan trọng; đảo 2 dây thì đổi chiều quay (xem mục 5, bước 6).

#### c) Receiver RC (bản RC FULL)

| Kênh RX | Chân board | Ghi chú |
|---|---|---|
| CH1 aileron (roll) | **PE2** | EXTI hai sườn |
| CH2 elevator (pitch) | **PE3** | |
| CH3 throttle | **PE4** | |
| CH4 rudder (yaw) | **PE5** | |
| CH5 công tắc ARM | **PE6** | |
| GND / +5 V của RX | GND / 5V của board | **GND phải nối chung** với board |

PE2–PE6 là chân trống trên header P1 của board (đã đối chiếu UM1670, bảng 7). Bản UART TEST và GAMEPAD không dùng các chân này.

#### d) Các kết nối khác

| Chức năng | Chân / cổng | Ghi chú |
|---|---|---|
| Nạp code + terminal UART | Cổng **CN1 mini-USB "USB ST-LINK"** (USART1 PA9/PA10 nối sẵn trên board) | 115200 8N1, cổng COM ảo |
| Tay cầm USB (bản GAMEPAD) | Cổng **CN6 micro-USB "USB USER"** qua cáp OTG; PB14 D−, PB15 D+, PB13 VBUS, **PC4** bật nguồn 5 V | **Không** cắm máy tính vào CN6 ở bản GAMEPAD (board xuất 5 V ra cổng này) |
| LCD ILI9341 | SPI5: PF7 SCK, PF9 SDA, PC2 CS, PD13 D/C | Có sẵn trên board, không cần đi dây |
| LED trạng thái | PG14 (LED đỏ LD4) | |
| Nguồn board khi bay | Chân **5V** + **GND** từ BEC 5V 3A | |

#### e) Chân đang bị dùng chung / cần tránh

| Chân | Dùng cho | Lưu ý |
|---|---|---|
| **PC4** | Bật nguồn USB (bản GAMEPAD); trước đây là `BLDC3_VCC` | **Không cắm gì vào PC4** (M3 đã dời sang PB2/PF11) |
| PB2 | Chân giữa (5 V) của giắc M3 | Cũng là chân BOOT1; chỉ có tác dụng khi BOOT0 = 1, nên không ảnh hưởng khởi động |
| PF11 | GND của M3 | Cũng là chân SDRAM trên board; an toàn vì firmware không dùng SDRAM |
| PG15, PE12, PE14, PF4, PF5, PF0, PF1 | Motor/MPU **và** chân SDRAM trên board | An toàn vì firmware không dùng SDRAM |
| PG13 | GND của M1 **và** LED xanh LD3 | LED xanh tắt là bình thường |
| PA8 | Tín hiệu M2 **và** I2C3 của màn cảm ứng (có điện trở kéo lên trên board) | Không ảnh hưởng xung PWM |

**Về màn hình:** LCD của DISC1 bình thường chạy LTDC + SDRAM, nhưng các chân đó đã được dùng cho motor (PB0, PB10, PA6, PC6, PE12, PE14, PG15) và MPU6050 (PF0, PF1, PF4, PF5). Vì vậy firmware ghi thẳng vào GRAM của ILI9341 qua SPI5. SPI5 nằm trên APB2 = 4.5 MHz nên SCK chỉ được 2.25 MHz; giao diện vì thế chỉ vẽ lại phần thay đổi. Nếu ảnh bị lật hoặc ngược, đổi `LCD_MADCTL_VALUE` trong `USER_Config.h`.

```
        M1 (FL, CW)    M3 (FR, CCW)
                 \  ▲  /
                  \ │ /      ▲ = mũi (hướng bay tới)
                  /   \
                 /     \
        M2 (BL, CCW)   M4 (BR, CW)
```

Chiều quay theo khung thực tế (kiểu "props out"): **M1, M4 quay CW; M2, M3 quay CCW** (nhìn từ trên xuống). Cột yaw của mixer (`MIX_Mx_YAW` trong `USER_Config.h`) đã được đặt theo chiều quay này: muốn mũi quay sang phải thì tăng tốc cánh CCW (M2, M3). Nếu sau này đổi chiều cánh, đổi dấu cột yaw.

---

## 2. Kiến trúc firmware

```
Task BLDCTask (High, 250 Hz)          Task UARTTask (Normal)   Task systemTask   Task uiTask (BelowNormal, 10 Hz)
  MPU6050 → đổi trục FRD                 lệnh @QC+...            LED trạng thái     LCD: trạng thái, chân trời
  → Attitude (complementary filter)      telemetry 20 Hz                            nhân tạo, motor, RC/setpoint
  → state machine (arm/disarm/failsafe)
  → PID góc (P) → PID tốc độ góc (PID)
  → Mixer Quad-X → ESC (µs)
```

### Màn hình LCD

```
+------------------------------+
| QUAD F450           RC FULL  |  tiêu đề + phiên bản build
| DISARMED                     |  trạng thái (vàng / xanh lá ARMED / đỏ FAILSAFE / cyan CALIB)
| +----------+  ROLL   -12.3   |
| |  ---o--- |  PITCH    4.5   |  chân trời nhân tạo + góc
| +----------+  YAW     30.0   |
| M1 1500 [########        ]   |  4 thanh motor (xanh lá khi ARMED)
| RC INPUT / UART SETPOINT     |
| last event                   |
| dt 4.0ms  IMU err 0          |
+------------------------------+
```

Quy ước trục (FRD): **roll > 0** là cánh phải chúi xuống, **pitch > 0** là ngóc mũi lên, **yaw > 0** là mũi quay sang phải.

| File (Core/) | Vai trò |
|---|---|
| `Inc/USER_Config.h` | **Mọi tham số chỉnh được**: gain PID, giới hạn, trục IMU, dấu mixer, kênh RC |
| `Src/USER_System.c` | Lõi điều khiển bay: state machine, vòng cascade, an toàn |
| `Src/USER_Attitude.c` | Hiệu chuẩn gyro, complementary filter |
| `Src/USER_PID.c` | PID (dt tường minh, anti-windup, D-on-measurement + lọc) |
| `Src/USER_Mixer.c` | Trộn Quad-X, ưu tiên giữ thăng bằng khi bão hòa |
| `Src/USER_Motor_Driver.c` | Xuất xung ESC theo µs, tự tính prescaler từ clock thật |
| `Src/USER_RC.c` | Đọc RX bằng EXTI + bộ đếm DWT (không tốn timer) |
| `Src/USER_MPU.c` | Driver MPU6050 |
| `Src/USER_Protocol.c` | Lệnh terminal |
| `Src/USER_DataCollect.c` | Telemetry |
| `Src/USER_LCD.c`, `Src/USER_Font8x8.c` | Driver ILI9341 qua SPI5 + font 8×8 |
| `Src/USER_UI.c` | Giao diện màn hình (task riêng, priority thấp) |
| `Src/USER_Gamepad.c` | Bản GAMEPAD: driver tay cầm USB (XInput / Xbox One / HID), task USB, chuyển clock sang HSE |
| `Src/usbh_conf.c`, `Inc/usbh_conf.h` | Lớp nối thư viện ST USB Host với cổng OTG CN6 (PB13–15, PC4) |

### Những gì đã sửa so với bản trước

- **PID chưa bao giờ chạy:** `PID_Gyro_Init` nhận struct theo giá trị nên PID trỏ vào biến tạm, gain bằng 0, và `Sys_InnerLoop` không được gọi ở đâu.
- **Tần số PWM sai:** TIM2/TIM3 chạy 18 MHz chứ không phải 9 MHz, nên "50 Hz" thực ra là 100 Hz. Đây chính là hiện tượng "motor 2 khác các motor còn lại" ghi ở slide 12. Giờ tất cả là 400 Hz, độ phân giải 1 µs.
- **Không có ước lượng góc:** trước đây giá trị gyro (°/s) bị in ra như góc.
- **I2C:** `MPU6050_I2C_ReInit` chỉ DeInit mà không Init lại, nên một lỗi BUSY làm I2C chết hẳn. Đã bỏ việc in lỗi ở mỗi lần đọc. DLPF đổi từ 260 Hz xuống 42 Hz để lọc rung motor. Dải đo gyro tăng lên ±500 °/s.
- **Lỗi khai báo:** thiếu `<stdint.h>` trong ring buffer, `RTOSmalloc` thiếu khai báo.
- **Mới thêm:** đọc RC, arm/disarm, failsafe, lệnh tune PID, telemetry, LED trạng thái.

Bản gốc đã được sao lưu tại `Code/_backup_Core_2026-11-21/`.

---

## 3. Build & nạp

### 3.1 Build

**Cách A – dòng lệnh (MSYS2), đã dùng để build và nạp thử trên board:**

```bash
# Cài 1 lần (trong terminal MSYS2 UCRT64):
pacman -S --needed mingw-w64-ucrt-x86_64-arm-none-eabi-gcc mingw-w64-ucrt-x86_64-arm-none-eabi-newlib make

# Build (đứng trong thư mục "Code/1. Control BLDC"):
export PATH=/c/msys64/ucrt64/bin:/c/msys64/usr/bin:$PATH
make -j8 DIRECT_LD=1                # bản RC FULL      -> build/quadcopter.bin
make -j8 DIRECT_LD=1 VARIANT=2      # bản UART TEST
make clean                          # khi đổi VARIANT hoặc UI, clean trước rồi build lại
```

`DIRECT_LD=1` chỉ cần trên máy bật **Smart App Control**: chính sách này chặn `collect2.exe` và `arm-none-eabi-size.exe` của MSYS2. Khi đó `tools/gcc-link.sh` gọi thẳng `arm-none-eabi-ld.exe` (không bị chặn) với đúng tham số gcc định dùng. Trên máy khác chỉ cần `make`.

**Cách A2 – Keil MDK (Arm Compiler 6), dùng trên máy phát triển từ 23/11/2026.** Từ ngày này, Smart App Control chặn luôn `arm-none-eabi-gcc.exe` của MSYS2. Bộ trình biên dịch của Keil do Arm ký số nên vẫn chạy được:

```bash
# đứng trong "Code/1. Control BLDC", cần Keil MDK tại C:\Keil_v5
sh tools/keil/build_keil.sh 3                       # bản GAMEPAD   -> build_keil/quadcopter.bin
sh tools/keil/build_keil.sh 1                       # bản RC FULL
sh tools/keil/build_keil.sh 3 1 "-DGPS_ENABLE=1"    # thêm cờ -D (ví dụ bật GPS)
```

Script dùng `tools/keil/startup_armclang.s` (sinh tự động từ file startup GCC, bảng vector giữ nguyên), `tools/keil/quadcopter.sct` và thư viện microlib (`printf` đi qua hàm `fputc()` trong `main.c`). Makefile GCC giữ nguyên để dùng trên máy khác.

**Cách B – STM32CubeIDE:** mở workspace `Code/` → project **1. Control BLDC** → Build. Các file `.c` mới trong `Core/Src` được tự động thêm vào build. Nếu mở lại `.ioc` và generate code, mọi thay đổi đã nằm trong vùng `USER CODE` hoặc đã được ghi vào `.ioc`, nên không bị mất.

### 3.2 Nạp (không cần driver)

ST-LINK/V2-1 trên DISC1 hiện ra như một **ổ USB tên `DIS_F429ZI`**. Chỉ cần:

1. Cắm cổng **USB ST-LINK (mini-USB, CN1)** vào PC.
2. Kéo-thả (hoặc copy) `build/quadcopter.bin` vào ổ `DIS_F429ZI`.
3. Đèn LD1 (COM) nháy đỏ/xanh trong lúc nạp; ổ đĩa tự ngắt rồi gắn lại; chip tự reset và chạy firmware mới.
4. Nạp **thành công** khi trong ổ **không có** file `FAIL.TXT`. Nếu có, mở file đó để xem lý do.

```powershell
Copy-Item "Code\1. Control BLDC\build\quadcopter.bin" E:\     # E: = ổ DIS_F429ZI
```

Các cách khác: CubeIDE (Run/Debug) hoặc STM32CubeProgrammer. Cả hai đều cần **driver ST-LINK (STSW-LINK009)**; trên máy phát triển, Device Manager đang báo "ST-Link Debug" lỗi mã 28 vì chưa có driver này. Nạp kéo-thả thì không cần.

---

## 4. Lệnh terminal (USART1, 115200, kết thúc bằng `\r\n`)

| Lệnh | Ý nghĩa |
|---|---|
| `@QC+HELP` | Danh sách lệnh ngắn gọn |
| `@QC+HELP=<TÊN>` | Cú pháp chi tiết và ví dụ của một lệnh, ví dụ `@QC+HELP=PID` |
| `@QC+LIST` | **Toàn bộ lệnh** kèm cú pháp, tham số, ví dụ và phiên bản build dùng được |
| `@QC+STATUS` | Trạng thái, góc, RC, motor, thời gian vòng lặp |
| `@QC+DEBUG=<n>` | Stream 20 Hz: 0 tắt, 1 góc, 2 RC, 3 motor, 4 PID roll/pitch, 5 PID yaw, 6 dòng `$TLM` cho `drone_viewer.py` |
| `@QC+CALIB` | Hiệu chuẩn lại gyro (để yên khung) |
| `@QC+LEVEL` | Lấy tư thế hiện tại làm "mặt phẳng" (chỉ lưu RAM) |
| `@QC+PID?` | Xem toàn bộ gain |
| `@QC+PID=RR,0.7,0.5,0.015` | Đặt gain: `RA`/`PA` góc roll/pitch, `RR`/`PR`/`YR` tốc độ góc roll/pitch/yaw |
| `@QC+CONTROL_MOTOR=<1..4\|5>,<µs>` | Test motor khi DISARMED (tối đa 1500 µs, 5 = cả 4 motor). **Tháo cánh!** |
| `@QC+STOP_MOTOR` | Tắt mọi motor test |
| `@QC+SP=<thr>,<roll>,<pitch>,<yawrate>` | Setpoint (chỉ ở bản UART TEST, ga tối đa 1300 µs) |
| `@QC+ARM` / `@QC+DISARM` | Arm (chỉ ở bản UART TEST) / Disarm (luôn có hiệu lực, mọi bản) |
| `@QC+PADTEST=1` / `=0` / `?` | Bản GAMEPAD: **test cần gạt → motor** không cần MPU6050 và không cần arm (mở vòng, qua mixer). LCD hiện `STICK TEST` màu tím. Ga ≤ 1050 µs thì motor đứng yên, tối đa 1500 µs. Dừng bằng `PADTEST=0`, B/BACK, `DISARM`, `STOP_MOTOR` hoặc rút tay cầm. **THÁO CÁNH** |
| `@QC+PADLOG=1` / `=0` | Bản GAMEPAD: in nút bấm và chuyển động cần gạt khi có thay đổi (mặc định bật) |
| `@QC+GPTEST` | Bản GAMEPAD: tự kiểm tra bộ giải mã HID/XInput ngay trên chip (rút tay cầm trước khi chạy) |

Lệnh `@QC+SRC` đã bị bỏ: nguồn điều khiển giờ cố định theo phiên bản build (mục 0).

**Lưu ý: `CONTROL_MOTOR` đã đổi cú pháp.** Trước đây là `<motor>,<tần số>,<duty %>`, giờ là `<motor>,<độ rộng xung µs>`.

Gain chỉnh bằng lệnh chỉ lưu trong RAM. Khi đã tìm được bộ gain tốt, hãy ghi vào `USER_Config.h`.

### Mô phỏng / viewer 3D bằng Python (`host/`)

#### Các file

| File | Vai trò | Cần thư viện ngoài? |
|---|---|---|
| `fc_model.py` | Bản port từng dòng của bộ điều khiển firmware (PID, bộ lọc góc, mixer, arm/disarm, giữ vị trí GPS) và **mô hình vật lý 6 bậc tự do của khung F450**. Hằng số (gain, giới hạn, dấu mixer) được **đọc trực tiếp từ `USER_Config.h`** | Không |
| `sim_flight_controller.py` | **Test hồi quy tự động**: bộ điều khiển chạy vòng kín với mô hình vật lý, in `ok` hoặc `FAIL` cho từng mục | Không |
| `drone_viewer.py` | Viewer 3D: xem tư thế máy thật qua UART, lái bằng bàn phím, hoặc bay mô phỏng | `vpython`, `pyserial`, `numpy` |
| `3D-Visualize.py` | Script cũ, dùng với `DEBUG=1` | `vpython`, `pyserial` |

#### Cài đặt

1. Cài **Python 3.9 trở lên** từ python.org, nhớ tick "Add python.exe to PATH". Trên máy phát triển, lệnh `python` mặc định chỉ là lối tắt mở Microsoft Store; đã dùng thay bằng `C:\msys64\ucrt64\bin\python.exe` (3.14).
2. Kiểm tra: `python --version`.
3. Chỉ khi cần **viewer 3D**: `pip install -r host/requirements.txt`. Hai bài test bên dưới **không cần** bước này.

Mọi lệnh chạy từ thư mục gốc dự án (`QuadCorter Project`).

#### Test 1: test hồi quy bộ điều khiển (chạy trước mỗi lần nạp code)

```
python host/sim_flight_controller.py
```

Chạy khoảng 20–40 giây. Kết quả đúng (đã chạy ngày 24/11/2026):

```
Unit checks
  ok  : +roll -> left motors (M1, M2) faster [...]
  ok  : +pitch -> front motors (M1, M3) faster [...]
  ok  : saturation keeps differential [2000, 2000, 1600, 1600]
  ok  : PID P+I first step
Attitude control (250 Hz loop, rigid body held in the air)
  ok  : arm with throttle low
  ok  : recover 10/-8 deg -> err 0.17/0.28 deg
  ok  : track roll 20 / pitch -15 -> err 0.06/0.03, peak 20.0
  ok  : yaw rate 90 dps -> err 1.63 dps
  ok  : back to level -> 0.01 0.01 0.87
Take-off and hover (full 6-DOF model)
  ok  : climbs with throttle 1437 us -> altitude 2.25 m
  ok  : stays level near hover: err 0.00/0.00 deg, drift 0.00 m
  ok  : nose down 10 deg -> moves forward 2.41 m
GPS position hold (USER_PosHold.c port, 5 Hz noisy GPS)
  ok  : nose    0 deg: 5 m push -> back within 0.02 m, max tilt sp 8.6 deg
  ok  : nose   90 deg: 5 m push -> back within 0.02 m, max tilt sp 9.0 deg
  ok  : nose -135 deg: 5 m push -> back within 0.02 m, max tilt sp 10.5 deg
  ok  : 3 m/s side wind, 25 s -> holds within 0.01 m (...)
  ok  : heading 180 deg wrong -> runs away (31.6 m), test is meaningful
Negative control
  ok  : wrong roll sign diverges (peak ... deg, state FAILSAFE)
Telemetry parser
  ok  : parse $TLM
  ok  : parse legacy attitude line

ALL PASSED
```

Ý nghĩa từng nhóm:

| Nhóm | Kiểm tra gì | Nếu FAIL thì nghĩ tới |
|---|---|---|
| Unit checks | Dấu cột `MIX_Mx_ROLL/PITCH` đúng với vị trí motor; mixer dồn ga khi bão hòa; phép tính PID | Sai dấu trong bảng mixer của `USER_Config.h` |
| Attitude control | Khung treo trên gimbal (chỉ xoay): tự về thăng bằng sau khi bị nghiêng 10°, bám góc đặt, bám tốc độ yaw | Gain PID quá cao hoặc quá thấp, sai dấu cột yaw (`MIX_Mx_YAW`) |
| Take-off and hover | Mô hình đầy đủ có trọng lực: cất cánh, giữ thăng bằng, chúi mũi thì bay tới | Sai quy ước dấu pitch, giới hạn ga |
| GPS position hold | Giữ vị trí với GPS 5 Hz có nhiễu, ở 3 hướng mũi khác nhau và khi có gió ngang 3 m/s | Gain `POSHOLD_*`, công thức xoay theo hướng mũi trong `USER_PosHold.c` |
| Negative control | **Cố tình làm sai** (đảo dấu roll, sai hướng 180°) và yêu cầu máy **phải mất ổn định** | Nếu nhóm này FAIL thì mô phỏng không đủ nhạy để bắt lỗi; các test khác không còn đáng tin |
| Telemetry parser | Đọc đúng dòng `$TLM` và dòng `DEBUG=1` | Định dạng in trong `USER_DataCollect.c` đã đổi |

- Dòng cuối là `ALL PASSED` hoặc `FAILED (n)`. Mã thoát là 0 hoặc 1, nên dùng được trong script.
- Muốn thử một file cấu hình khác: `python host/sim_flight_controller.py đường/dẫn/USER_Config.h`.
- **Quy trình khi sửa code điều khiển:**
  1. Sửa `USER_Config.h` (gain, giới hạn, dấu mixer, `POSHOLD_*`).
  2. Chạy test này, phải ra `ALL PASSED`.
  3. Mới build và nạp.

  Nếu sửa **thuật toán** trong file C (`USER_PID.c`, `USER_Attitude.c`, `USER_Mixer.c`, `USER_PosHold.c`, `USER_System.c`), phải sửa **đúng như vậy** trong `fc_model.py`. Mô phỏng chỉ kiểm tra được bản port Python, không đọc trực tiếp code C.
- Test đạt nghĩa là **dấu đúng và gain hợp lý trên một mô hình ước lượng** (khung 450 mm, khối lượng 1.2 kg, lực đẩy motor ước lượng…). Nó **không thay thế** bài test trên bàn (tháo cánh) và test buộc dây.

#### Test 2: tự kiểm tra viewer (không mở cửa sổ)

```
python host/drone_viewer.py --selftest
```

Kiểm tra phần logic của viewer mà không cần vpython hay board: phím bấm ra lệnh đúng chiều (mũi tên lên = chúi mũi, D = yaw phải), nguồn mô phỏng arm được, cất cánh, nghiêng phải thì trôi sang Đông, rồi disarm. Kết quả đúng kết thúc bằng `SELFTEST PASSED`.

#### Test 3: bay mô phỏng có hình 3D (không cần board)

```
python host/drone_viewer.py --sim
```

Cần `pip install -r host/requirements.txt`. Bộ điều khiển firmware (bản port) và vật lý F450 chạy trên PC; bạn lái bằng bàn phím như lái máy thật. Đã chạy được trên máy phát triển ngày 24/11/2026.

> **Trên máy phát triển** (Smart App Control chặn `numpy` của MSYS2), viewer chạy bằng **Python đi kèm KiCad 8**, trong môi trường ảo `host/.venv` đã cài sẵn `vpython` và `pyserial`:
> ```
> host\.venv\Scripts\python.exe host\drone_viewer.py --sim
> host\.venv\Scripts\python.exe host\drone_viewer.py --port COM4
> ```
> Cách tạo lại môi trường ảo nếu bị xóa:
> ```
> "%LOCALAPPDATA%\Programs\KiCad\8.0\bin\python.exe" -m venv --system-site-packages host\.venv
> host\.venv\Scripts\python.exe -m pip install vpython pyserial
> ```
> Nếu trình duyệt không tự mở, xem cổng trong `netstat -ano` của tiến trình `pythonw` rồi mở `http://localhost:<cổng>`. Đóng tab trình duyệt thì viewer tự thoát.

1. Click vào khung 3D trong trình duyệt để nó nhận bàn phím.
2. `SPACE` để arm, `H` để đưa ga về mức hover, `W/S` tăng/giảm ga.
3. Mũi tên để nghiêng (lên = bay tới), `A/D` để xoay.
4. `R` để reset, `Q` để thoát.

#### Test 4: xem máy thật real-time

```
python host/drone_viewer.py --port COM4             # drone 3D xoay theo tư thế đọc từ UART
python host/drone_viewer.py --port COM4 --control   # + lái bằng bàn phím (firmware bản UART TEST, THÁO CÁNH)
```

Đóng mọi terminal đang mở COM4 trước khi chạy.

#### Ghi chú

- Viewer mở trong trình duyệt (vpython). Click vào khung 3D trước để nó nhận bàn phím.
- Phím: `SPACE` arm, `X` disarm, `W/S` tăng/giảm ga, mũi tên = pitch/roll, `A/D` = yaw, `H` ga hover (sim), `C` calib, `L` level, `R` reset (sim), `Q` thoát (tự gửi DISARM).
- Ở chế độ live, script tự gửi `@QC+DEBUG=6`. Cánh quạt đổi màu theo công suất motor (xanh dương đến đỏ).
- Ở chế độ `--control`, setpoint chỉ được gửi khi thay đổi, cộng thêm 1 lần mỗi giây; ga bị giới hạn theo `UART_SP_THROTTLE_MAX_US`.
- `fc_model.py` là bản port của bộ điều khiển firmware và đọc hằng số trực tiếp từ `USER_Config.h`. Đổi gain trong file đó thì mô phỏng dùng ngay gain mới.
- `3D-Visualize.py` (script cũ) vẫn dùng được với `DEBUG=1`.

---

## 5. Hướng dẫn test từng bước

Làm **theo đúng thứ tự**; chỉ sang bước sau khi bước trước đạt. Bước 1–5 chỉ cần cáp USB, **chưa cắm pin LiPo**. Từ bước 6 trở đi **tháo hết cánh quạt**.

LED đỏ LD4 cho biết trạng thái: **nháy nhanh** = CALIBRATING, **nháy chậm** = DISARMED, **sáng liên tục** = ARMED, **nháy đôi** = FAILSAFE.

### Bước 1 – Mở terminal

- Cổng: **USB Serial Device (COMx)** trong Device Manager. Đó là cổng ảo của ST-LINK, nối vào USART1 (PA9/PA10); trên máy phát triển là **COM4**.
- Cấu hình **115200, 8N1**, gửi kèm **CR+LF** (`\r\n`). Dùng PuTTY, Tera Term, Serial Monitor của VS Code, hoặc Arduino Serial Monitor (chọn "Both NL & CR").
- Nhấn nút **RESET (đen)** trên board để xem log khởi động.

**Đạt** khi thấy (log thật, đã chạy trên board):
```
> Build variant: RC FULL
> Motor init: holding 1000 us for 3000 ms (ESC arming)...
> Checking MPU6050 (I2C2 PF1=SCL PF0=SDA, 100000 Hz)...
> I2C2: device found at 0x68                       <- chỉ có khi đã nối MPU6050 (bước 3)
> MPU6050 configured (addr 0x68, gyro 65.5 LSB/dps, acc 4096 LSB/g)
ENTER FREERTOS
> Flight task started. Keep the frame still: calibrating gyro...
> QuadCopter FC ready. Send @QC+HELP for commands.
```
Gửi `@QC+HELP`: firmware phải trả về danh sách lệnh và `@ Command OK !!`.

**Không thấy gì?** Kiểm tra đúng cổng COM, đúng baud, rồi nhấn RESET. Nếu vẫn không có gì, nạp lại (mục 3.2) và kiểm tra ổ đĩa có `FAIL.TXT` không.

### Bước 2 – Màn hình LCD

Ngay sau khi khởi động, LCD phải hiện:

- thanh tiêu đề xanh đậm `QUAD F450` cùng `RC FULL` (hoặc `UART TEST`);
- trạng thái `CALIBRATING` màu cyan;
- khung chân trời nhân tạo, các nhãn `ROLL / PITCH / YAW`;
- 4 dòng `M1..M4 1000` kèm thanh xám;
- dòng sự kiện ở cuối màn hình.

| Hiện tượng | Xử lý |
|---|---|
| Trắng xóa / đen hoàn toàn | Board chưa nhận lệnh SPI. Nhấn RESET; kiểm tra `UI_LCD_ENABLE = 1`. Nếu vẫn vậy, báo lại kèm log UART |
| Hình bị lật gương / ngược | Đổi `LCD_MADCTL_VALUE` (thử 0x88, 0x08, 0xC8) |
| Màu đỏ ↔ xanh dương bị đảo | Đổi bit BGR: 0x48 → 0x40 |

### Bước 3 – Nối và kiểm tra MPU6050

Nối module GY-521 (MPU6050) với board:

| MPU6050 | Board |
|---|---|
| VCC | **PF5** (firmware xuất 3.3 V ra chân này) hoặc chân 3V |
| GND | **PF4** (firmware kéo xuống 0 V) hoặc GND |
| SCL | **PF1** |
| SDA | **PF0** |
| AD0 | để trống hoặc GND (0x68); nối VCC thì địa chỉ là 0x69, firmware tự nhận |

Nhấn RESET và đọc log:

| Log | Ý nghĩa |
|---|---|
| `device found at 0x68` + `MPU6050 configured` | **Đạt** |
| `SDA (PF0) has NO pull-up` / `SCL (PF1) has NO pull-up` | Module **chưa được nối hoặc chưa có nguồn** (đây là kết quả lúc kiểm tra trên máy phát triển, khi chưa cắm MPU). Kiểm tra dây VCC/GND |
| `no answer ... NACK` nhưng không có dòng "NO pull-up" | Có dây và có pull-up nhưng không có thiết bị trả lời: SDA/SCL bị đảo, hoặc module hỏng |
| `bus BUSY` / `timeout` | Dây lỏng hoặc chập; rút ra cắm lại rồi RESET |

Có thể cắm MPU **sau khi** đã khởi động: firmware tự dò lại mỗi 2 giây (sự kiện `IMU not found (retrying)` đổi thành `IMU found, calibrating`).

### Bước 4 – Hiệu chuẩn và kiểm tra trục

1. Để board **đứng yên** khoảng 2 s. Trạng thái chuyển `CALIBRATING` → `DISARMED` (LCD đổi sang màu vàng, LED nháy chậm).
2. `@QC+STATUS` phải báo `calibrated=1`, `IMU errors` không tăng nữa, `dt=4.00 ms`.
3. `@QC+DEBUG=1` rồi nghiêng thử board/khung:
   - nghiêng **cánh phải xuống** thì `R` **dương**;
   - **ngóc mũi lên** thì `P` **dương**;
   - **xoay mũi sang phải** thì `Y` **tăng**.

   Nếu sai, sửa `IMU_BODY_X/Y/Z` trong `USER_Config.h`, build và nạp lại. Chân trời trên LCD cũng phải nghiêng ngược chiều với khung, giống đồng hồ chân trời trong buồng lái.
4. Đặt khung trên mặt phẳng, gửi `@QC+LEVEL` (roll/pitch về khoảng 0). Gửi `@QC+DEBUG=0` để tắt stream.

### Bước 5 – Viewer 3D trên PC (tùy chọn)

Cần Python cho Windows (python.org) cùng `pip install -r host/requirements.txt`. **Đóng terminal serial trước**, vì mỗi lúc chỉ một chương trình mở được cổng COM.

```
python host/drone_viewer.py --port COM4
```

Trình duyệt mở cảnh 3D; nghiêng board thì drone trên màn hình nghiêng theo. Không có board thì dùng `--sim`.

### Bước 6 – ESC và motor (cắm pin, THÁO CÁNH)

1. Cắm pin LiPo **sau khi** board đã chạy: ESC kêu bíp xác nhận tín hiệu 1000 µs.
2. Lần lượt: `@QC+CONTROL_MOTOR=1,1150` … `=4,1150`. Mỗi lần chỉ đúng motor đó quay chậm (LCD: thanh `M1` dài ra).
3. Kiểm tra **vị trí** và **chiều quay** đúng sơ đồ: M1 trước-trái **CW**, M2 sau-trái **CCW**, M3 trước-phải **CCW**, M4 sau-phải **CW**. Quay ngược thì đảo 2 trong 3 dây motor.
4. `@QC+STOP_MOTOR` thì mọi motor dừng. Giới hạn test là 1500 µs.

Lệnh test motor chạy được cả khi chưa có MPU6050 (trạng thái CALIBRATING), nhưng không thể arm.

### Bước 7a – Bản RC FULL: tay điều khiển (THÁO CÁNH)

1. Nối RX: CH1→PE2, CH2→PE3, CH3→PE4, CH4→PE5, CH5→PE6, GND chung, nguồn 5 V cho RX.
2. Bật tay điều khiển, gửi `@QC+DEBUG=2`. Phải thấy `RC OK`, mọi kênh trong 1000–2000, ga thấp nhất < 1050, CH5 bật > 1700. Đẩy cần pitch tới thì `SP P` âm; cần roll sang phải thì `SP R` dương. Nếu ngược, đổi `RC_REVERSE_*`.
3. **Arm:** ga về thấp nhất, gạt CH5 lên. LCD hiện `ARMED` màu xanh lá, LED sáng liên tục, motor quay ở mức idle 1100 µs.
4. **Kiểm tra phản ứng** bằng `@QC+DEBUG=3`, ga khoảng 1300:
   - nghiêng khung **sang phải** thì **M3, M4 (bên phải) tăng**;
   - **ngóc mũi lên** thì **M2, M4 (phía sau) tăng**.

   Nếu ngược, đổi dấu cột `MIX_Mx_*` tương ứng.
5. **Failsafe:** đang ARMED thì tắt tay điều khiển. Motor phải dừng ngay, LCD hiện `FAILSAFE` màu đỏ, LED nháy đôi. Bật lại tay điều khiển, gạt CH5 về OFF thì trạng thái về DISARMED.
6. Gạt CH5 xuống thì máy disarm.

### Bước 7b – Bản UART TEST: không có tay điều khiển (THÁO CÁNH)

Build với `make DIRECT_LD=1 VARIANT=2` rồi nạp. LCD hiện `UART TEST` màu cam.

```
@QC+ARM                      -> ARMED (motor idle 1100 us)
@QC+SP=1250,0,0,0            -> ga 1250, giữ thăng bằng
@QC+DEBUG=3                  -> nghiêng khung, kiểm tra motor phản ứng như bước 7a.4
@QC+SP=1250,10,0,0           -> setpoint roll 10 deg: M1, M2 (bên trái) tăng
@QC+DISARM                   -> dừng
```

Hoặc lái bằng bàn phím: `python host/drone_viewer.py --port COM4 --control` (SPACE arm, W/S ga, mũi tên, X disarm, Q thoát). Nên đặt `UART_SP_TIMEOUT_MS = 3000` để máy tự disarm khi mất kết nối.

### Bước 7c – Bản GAMEPAD: tay cầm USB (THÁO CÁNH)

**1. Phần cứng cần có**

- Cáp/đầu chuyển **USB OTG**: micro-USB (đực, cắm vào **CN6**, cổng micro-USB "USB USER" ở cạnh dưới board) sang USB-C (cắm vào tay cầm). Đầu chuyển phải là loại **OTG** (chân ID nối đất); cáp sạc micro-USB thông thường không dùng được.
- Board phải được cấp **5 V**: qua cáp ST-LINK (CN1), hoặc từ BEC vào chân 5V khi đã gắn lên drone. Board cấp 5 V cho tay cầm qua CN6; đèn **LD5 (xanh lá) sáng** nghĩa là đã có VBUS.
- **Rút mọi dây đang cắm vào PC4** (trước đây là `BLDC3_VCC`).

**2. Build và nạp**

```bash
make clean && make -j8 DIRECT_LD=1 VARIANT=3      # rồi copy build/quadcopter.bin vào ổ DIS_F429ZI
```

Log khởi động phải có `Build variant: GAMEPAD` và `USB host: clock HSE 8MHz (...)`. LCD hiện `GAMEPAD` màu cyan và dòng `NO PAD - plug into CN6`.

Kiểm tra bộ giải mã (chưa cắm tay cầm): gửi `@QC+GPTEST`. Các dòng `HID parse`, `HID decode`, `XInput`, `Switch Pro` phải là **PASS**.

**3. Chọn chế độ của tay cầm Nova 2 Lite**

Firmware hỗ trợ cả 4 kiểu: **chế độ Switch** (giao thức Switch Pro), **XInput (Xbox/PC)**, Xbox One và HID gamepad chuẩn.

- **Chế độ Switch (đèn đỏ)**: **đã chạy thực tế** với Nova 2 Lite trên board (2026-11-21). Tay cầm báo `VID 057E PID 2009`, firmware gửi chuỗi bắt tay USB rồi nhận report liên tục khoảng 125 lần/giây. Log mong đợi:
  ```
  > USB: gamepad SWITCH ready (VID 057E PID 2009)
  > USB: Switch Pro handshake sent, waiting for 0x30 reports
  ```
  Ở chế độ này, nút được gán **theo vị trí** giống tay Xbox: nút dưới (B của Nintendo) = "A", nút phải (A của Nintendo) = "B", **+ = START**, **− = BACK**, ZL/ZR = trigger (bật/tắt).
- **Chế độ Xbox/PC (đèn xanh dương)**: log là `USB: gamepad XINPUT ready`. Theo manual GameSir, giữ **Menu + View 2 giây** để đổi qua lại Xbox/Switch (một số tài liệu ghi **Home + X**).
- LCD hiện `SWITCH OK  START=arm B=stop` (hoặc `XINPUT OK ...`).
- Nếu log ghi `NOT supported` kèm class (ví dụ `03/00/00` là HID, `FF/5D/01` là XInput), tay cầm đang ở chế độ khác. Đổi chế độ rồi cắm lại, và gửi dòng log đó cho người phát triển để thêm hỗ trợ.

**4. Kiểm tra cần và nút:** gửi `@QC+DEBUG=2`.

- Cần trái sang phải: `LX` tiến về +1. Cần trái lên trên: `LY` tiến về +1. Cần phải tương tự với `RX`/`RY`.
- Bóp trigger thì `LT`/`RT` từ 0 lên 1. Mỗi nút bấm làm thay đổi `BTN`.
- Với tay cầm HID, cột `RAW` cho biết số thứ tự nút. Nếu nút bị map sai, sửa `GAMEPAD_HID_BTN_*` trong `USER_Config.h`.

**5. Bảng điều khiển** (giống tay RC "mode 2")

| Tay cầm | Chức năng |
|---|---|
| **Giữ START (Menu / +) 0.5 s** | **ARM** (ga phải ở mức thấp nhất, khung phẳng, gyro đã hiệu chuẩn) |
| **B** hoặc **BACK (View / −)** | **DISARM ngay** (dừng khẩn cấp). Ở chế độ Switch, "B" là **nút bên phải** của cụm 4 nút |
| Cần trái lên/xuống | **Ga**: đẩy lên thì ga tăng dần, thả cần về giữa thì **giữ nguyên** ga, kéo xuống thì giảm |
| Cần trái trái/phải | Xoay (yaw) |
| Cần phải trái/phải | Nghiêng (roll) |
| Cần phải lên/xuống | Chúi mũi / ngóc mũi (pitch; đẩy lên = bay tới) |
| X (khi chưa arm) — nút **trái** của cụm 4 nút | Hiệu chuẩn gyro |
| Y (khi chưa arm) — nút **trên** của cụm 4 nút | Lưu mặt phẳng (level) |

Cần tay cầm tự bật về giữa, nên ga mặc định là kiểu "giữ": cần trái chỉ **thay đổi** ga với tốc độ 400 µs/giây. Mỗi lần arm/disarm, ga trở về mức thấp nhất. Muốn dùng **trigger RT làm ga tuyệt đối** (thả là về 0, kiểu "dead-man"), đặt `GAMEPAD_THROTTLE_MODE 1`.

**6. Bài test (tháo cánh, cắm pin)**

1. Giữ START: LCD hiện `ARMED`, motor quay idle 1100 µs.
2. Đẩy cần trái lên khoảng 1 giây: dòng `T` trên LCD tăng lên khoảng 1400. Thả cần, ga phải **giữ nguyên**.
3. `@QC+DEBUG=3`: nghiêng khung và đẩy cần phải, motor phải phản ứng giống bước 7a.4.
4. Bấm **B**: motor dừng, trạng thái về `DISARMED`.
5. **Failsafe:** arm lại, rồi **rút tay cầm khỏi CN6**. Motor phải dừng ngay, LCD hiện `FAILSAFE: gamepad disconnected`. Cắm lại tay cầm (không giữ START) thì trạng thái về `DISARMED`.

### Bước 8 – Lắp cánh (chỉ sau khi bước 1–7 đều đạt)

Buộc khung vào giá cố định hoặc dây (một trục), rồi tune theo mục 6. Không bay tự do cho tới khi khung đứng ổn định trên giá.

### Bảng xử lý sự cố nhanh

| Hiện tượng | Nguyên nhân thường gặp |
|---|---|
| Kẹt ở `CALIBRATING` | Không có MPU (xem bước 3) hoặc khung bị rung/di chuyển trong lúc hiệu chuẩn |
| `IMU errors` tăng liên tục | Dây I2C lỏng; thử `MPU6050_I2C_SPEED_HZ` 100000 |
| `REFUSED: Frame is tilted` | Khung nghiêng > 25°, hoặc chưa `@QC+LEVEL` |
| `REFUSED: Throttle not low` | Ga > 1050 µs |
| Arm bằng CH5 không ăn | Công tắc đã bật sẵn lúc khởi động: gạt OFF rồi ON lại |
| Motor không quay khi test | Chưa cắm pin, ESC chưa nhận 1000 µs lúc bật nguồn, hoặc đang ARMED/FAILSAFE |
| Lệnh không phản hồi | Thiếu CR+LF ở cuối lệnh; viewer Python đang chiếm cổng COM |
| (GAMEPAD) LCD `NO PAD`, LD5 xanh không sáng | Board chưa có 5 V, cáp không phải OTG, hoặc chưa nạp bản `VARIANT=3` |
| (GAMEPAD) `NOT supported` | Tay cầm ở chế độ lạ (Android/iOS…): chuyển sang Switch (đèn đỏ) hoặc Xbox/PC (đèn xanh dương) rồi cắm lại |
| (GAMEPAD) `SWITCH ready` nhưng `reports` không tăng | Chuỗi bắt tay chưa thành công: rút ra cắm lại; nếu vẫn vậy, chuyển sang Xbox/PC |
| (GAMEPAD) arm không ăn | Phải nhả START rồi giữ lại 0.5 s; khung phải phẳng, gyro đã hiệu chuẩn |

### Điều kiện arm (bằng công tắc CH5, sườn lên)

Gyro đã hiệu chuẩn, nghiêng < 25°, ga < 1050 µs, tín hiệu RC tốt. Nếu công tắc đang bật sẵn lúc khởi động hoặc lúc tín hiệu RC vừa có lại, phải gạt về OFF rồi mới arm được.

### Các trường hợp tự disarm

Gạt công tắc OFF, mất tín hiệu RC > 100 ms, nghiêng > 60° (coi là rơi), lỗi đọc IMU liên tiếp 10 lần. Gặp các lỗi này, máy vào FAILSAFE và chỉ thoát khi công tắc về OFF.

---

## 6. Tune PID (thứ tự gợi ý)

Gain mặc định trong `USER_Config.h` chỉ là **điểm khởi đầu**. Chúng đã được kiểm tra ổn định trên mô hình F450 ước lượng (`host/sim_flight_controller.py`), nhưng phải tune lại trên khung thật.

1. Buộc khung trên trục quay một bậc tự do (dây hoặc thanh ngang) để chỉ roll được, cánh lắp vào, ga khoảng 1400–1500.
2. **Vòng tốc độ góc trước:** tạm đặt `RA` cao (khoảng 4) và `Ki = 0, Kd = 0` cho vòng `RR`. Tăng `Kp` cho tới khi bắt đầu rung nhanh, rồi lùi lại 30–50%.
3. Thêm `Kd` (0.01–0.03) để giảm vọt lố, sau đó thêm `Ki` (0.3–1.0) để khử trôi.
4. Làm tương tự cho pitch (`PR`), rồi yaw (`YR`, thường chỉ cần P và I).
5. **Vòng góc** (`RA`/`PA`): tăng cho tới khi phản ứng nhanh mà không dao động chậm.
6. Dùng `@QC+DEBUG=4` để xem từng thành phần P/I/D.

Sau khi đổi gain hoặc dấu mixer trong `USER_Config.h`, chạy `python host/sim_flight_controller.py` để kiểm tra nhanh rằng hệ vẫn ổn định và đúng dấu.

---

## 6b. GPS và chế độ giữ vị trí (POSHOLD), tùy chọn

Bật bằng `#define GPS_ENABLE 1` trong `USER_Config.h`. Mặc định là `0`: không có code GPS nào được biên dịch, firmware giống hệt bản trước.

**Phần cứng** (module u-blox NEO-6M / M8N / M10, mức logic 3.3 V):

| Module GPS | Board DISC1 | Ghi chú |
|---|---|---|
| TX | **PD2** (P1-40), UART5_RX | |
| RX | **PC12** (P1-43), UART5_TX | |
| VCC | 5V (P1-1/2) | Module có ổn áp riêng |
| GND | GND | |
| SDA / SCL (la bàn QMC5883L trên module M8N) | **PF0 / PF1**, dùng chung I2C2 với MPU6050 | Chỉ khi `GPS_HEADING_SOURCE 0` |

PC12 và PD2 là chân "free I/O" của DISC1 (UM1670, bảng 7).

**Thuật toán** (`USER_PosHold.c`). Nó đứng **trước** vòng góc hiện có; vòng góc, vòng tốc độ góc và mixer giữ nguyên:

```
sai lệch vị trí (m, Bắc/Đông) × POSHOLD_POS_P  → vận tốc mong muốn (≤ POSHOLD_MAX_SPEED_MS)
sai lệch vận tốc × POSHOLD_VEL_P + I (bù gió)   → góc nghiêng mong muốn theo Bắc/Đông
xoay theo hướng mũi (la bàn)                    → roll / pitch setpoint (≤ POSHOLD_MAX_ANGLE_DEG)
```

- Ga và yaw **vẫn do người lái**: chưa có cảm biến áp suất nên chưa giữ được độ cao.
- Gạt cần phải ra khỏi vùng chết thì người lái điều khiển; thả cần về giữa thì máy giữ tại vị trí vừa dừng.
- Mất GPS hoặc mất hướng: chỉ **tạm ngưng giữ vị trí** và quay lại chế độ giữ góc bình thường. Không bao giờ tắt motor vì lý do này.
- Kiểm chứng bằng mô phỏng `host/sim_flight_controller.py`: giữ vị trí đúng với mọi hướng mũi, bù được gió ngang 3 m/s. Test đối chứng (sai hướng 180°) thì máy bị trôi.

**Hướng mũi** (`GPS_HEADING_SOURCE`):
- `0`: dùng la bàn QMC5883L. Chạy `@QC+MAGCAL` một lần, rồi xoay máy đủ mọi hướng trong 30 giây. Kiểm tra bằng `@QC+GPS?`: quay mũi về hướng Bắc thì `Heading` phải gần 0, quay về Đông thì gần 90. Nếu sai, sửa `MAG_BODY_X/Y/Z`.
- `1`: không có la bàn. Quay mũi về **hướng Bắc** trước khi arm. Hướng mũi lúc đó được tính bằng tích phân gyro nên trôi vài độ mỗi phút; chỉ dùng để test ngắn.

**Dùng:**
1. Arm, cất cánh, giữ độ cao bằng ga.
2. Bấm **RB** trên tay cầm, hoặc gửi `@QC+POSHOLD=1`.
3. Chế độ giữ vị trí chỉ kích hoạt khi có fix 3D, ≥ `GPS_MIN_SATS` vệ tinh, HDOP ≤ `GPS_MAX_HDOP` và có hướng mũi.
4. Xem trạng thái bằng `@QC+GPS?` hoặc `@QC+STATUS`.

Lần đầu thử: chỉ thử khi máy đã bay ổn định ở chế độ thường, ở bãi trống, gió nhẹ. Tay luôn sẵn sàng bấm **RB** (tắt giữ vị trí) hoặc **B** (disarm).

**Build:** nếu chỉ muốn thử mà không sửa config, chạy `sh tools/keil/build_keil.sh 3 1 "-DGPS_ENABLE=1"`.

---

## 7. Giới hạn hiện tại / việc còn lại

- **Đã kiểm chứng trên board (2026-11-21, chưa nối MPU6050, ESC hay RX):**
  - build bằng arm-none-eabi-gcc 16.1, không có cảnh báo;
  - nạp kéo-thả thành công;
  - FreeRTOS chạy, vòng điều khiển đúng 4.00 ms;
  - lệnh UART, telemetry `$TLM` và test motor (đổi độ rộng xung) hoạt động;
  - chẩn đoán I2C báo đúng là "không có thiết bị".
- **Chưa kiểm chứng:** hiển thị LCD (cần người nhìn màn hình), MPU6050, RC, ESC thật, phản ứng điều khiển. Hãy làm theo mục 5.
- **GPS / giữ vị trí** chưa làm: chưa có module GPS nào được nối và cấu hình. Outer loop GPS trước đây bị lỗi nên đã bỏ; `USER_GPS.h` chỉ còn là chỗ để dành.
- Không có barometer, nên **không giữ độ cao**. Ga điều khiển hoàn toàn bằng tay.
- Yaw là tích phân gyro nên bị trôi. Điều này không ảnh hưởng điều khiển (vòng yaw là vòng tốc độ góc), chỉ ảnh hưởng hiển thị.
- Gain và level trim chưa được lưu vào flash; khởi động lại sẽ quay về giá trị trong `USER_Config.h`.
- Còi (buzzer) chưa được dùng.
- **Màn hình chưa được thử trên board.** Chuỗi khởi tạo ILI9341 là chuỗi chuẩn cho giao tiếp SPI, chân lấy theo UM1670. Nếu màn hình trắng hoặc đen, kiểm tra lại PC2/PD13 và `LCD_MADCTL_VALUE`.
- Bản UART TEST mặc định không có failsafe mất liên lạc (`UART_SP_TIMEOUT_MS = 0`, để gõ lệnh tay được): nếu cáp USB-UART rút ra khi đang ARMED, motor giữ setpoint cuối (ga ≤ 1300 µs). Khi dùng `drone_viewer.py --control` (gửi keepalive mỗi giây), nên đặt `UART_SP_TIMEOUT_MS` = 3000. Chỉ dùng khi đã tháo cánh hoặc buộc khung cố định.
- `drone_viewer.py` mới chỉ được kiểm tra phần logic (`--selftest`); phần vẽ vpython chưa chạy thử được trên máy phát triển.
- **Bản GAMEPAD, đã kiểm chứng trên board (2026-11-21):**
  - clock HSE, USB host enumerate Nova 2 Lite ở **chế độ Switch** (057E:2009);
  - bắt tay Switch Pro thành công, report liên tục, cần trái đo được đủ −1…+1;
  - self-test bộ giải mã HID/XInput PASS trên chip.
- **Bản GAMEPAD, chưa kiểm chứng:** nút bấm trên tay cầm thật (chưa có ai bấm trong lúc đo), chế độ XInput với tay cầm thật, và arm/bay thật (vì chưa có MPU6050).
  - Thư viện USB Host là bản chính thức của ST (v3.3.4, khớp với HAL V1.7.13). Có một bản vá nhỏ trong `usbh_ctlreq.c`, có ghi chú "QuadCopter patch", để chặn ghi tràn mảng endpoint.
  - Nếu generate lại code từ `.ioc` trong CubeMX, kiểm tra lại dòng `#define HAL_HCD_MODULE_ENABLED` trong `stm32f4xx_hal_conf.h`, vì CubeMX có thể comment nó đi.
