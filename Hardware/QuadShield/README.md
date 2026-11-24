# QuadShield – mạch đế cho STM32F429I-DISC1 (QuadCopter F450)

Shield 2 lớp, kích thước **66 × 155 mm**, cắm **bên dưới** board DISC1:
- Hai socket cái 2x32 nhận header P1/P2 của DISC1.
- Phần trên của shield (0–119.3 mm) trùng đúng khuôn board DISC1.
- Phần mở rộng phía dưới (119.3–155 mm) chứa các linh kiện cao: XT30, còi, giắc ESC/RC/GPS.

## Chức năng

| Khối | Chi tiết | Chân DISC1 |
|---|---|---|
| Nguồn | XT30 (pin 2S–4S) → PTC 1.5 A → SS34 chống ngược cực → **AP63205** 5 V / 2 A → SS34 (diode OR) → chân 5V của DISC1 | P1-1/2 (5V) |
| ESC ×4 | Giắc 3 chân: **S / để trống / GND**. Dây đỏ 5V của ESC không nối, vì trước đây nó gây reset MCU | M1 PB4 (P1-25), M2 PA8 (P1-53), M3 PB0 (P2-28), M4 PB10 (P2-48) |
| MPU6050 | GY-521 **hàn nằm phẳng** (cao khoảng 5 mm, vừa khe dưới DISC1). **Mũi tên X hướng về đuôi máy**, khớp với `IMU_BODY_*` hiện tại | SDA PF0 (P1-7), SCL PF1 (P1-8), VCC 3V (P2-1) |
| GPS | Giắc 1x6: 5V, TX, RX, SCL, SDA, GND (dạng module M8N) | GPS TX → PD2 (P1-40), GPS RX ← PC12 (P1-43) |
| RC | 5 giắc 3 chân S / + / − | PE2–PE6 |
| Đo áp pin | 100k / 22k + 100 nF (4S 16.8 V cho ra 3.03 V) | PC3 (P2-15) |
| Còi | Còi active 5 V, transistor MMBT3904, diode chống ngược | PD4 (P1-38) |
| Pull-up I2C | R6/R7 4k7 **DNP** (không hàn), vì GY-521 đã có sẵn | |

Nguồn số liệu:
- Pinout và kích thước cơ khí: UM1670 Rev 5 (bảng 7, hình 3–5), file trong `Documents/`.
- Mạch buck: datasheet Diodes DS41326, hình 1 (FB nối VOUT, EN nối VIN, 4.7 µH, Cin 10 µF, Cout 2×22 µF, C BST 100 nF).

## File

| File | Nội dung |
|---|---|
| `design.py` | **Nguồn duy nhất**: linh kiện, footprint, net, bảng chân P1/P2 |
| `gen_sch.py` | Sinh `QuadShield.kicad_sch` (ký hiệu lấy từ thư viện KiCad 8, nối bằng nhãn net) |
| `gen_pcb.py` | Đặt linh kiện, router A* 2 lớp, đổ GND hai mặt, via khâu → `QuadShield.kicad_pcb` |
| `fill_zones.py` | Tô vùng đồng (tách riêng vì `ZONE_FILLER` crash trên board tạo bằng script) |
| `build.ps1` | Chạy cả chuỗi, xuất ERC/DRC. Thêm `-Fab` để xuất Gerber, file khoan, BOM, file vị trí, PDF vào `fab/` |
| `fab/QuadShield-gerbers.zip` | Gói gửi nhà mạch (JLCPCB, PCBWay…) |

Build lại sau khi sửa `design.py` hoặc phần đặt linh kiện (`PLACE`) trong `gen_pcb.py`:

```
powershell -ExecutionPolicy Bypass -File build.ps1 -Fab
```

Kết quả hiện tại (24/11/2026):
- **ERC:** 0 lỗi điện. Có 43 cảnh báo "library not in configuration", sẽ tự hết sau lần đầu mở KiCad.
- **DRC:** 0 vi phạm, 0 pad chưa nối, khớp 100% với schematic.

## Phải kiểm tra trước khi đặt mạch

1. **Mở KiCad → PCB Editor → View → 3D Viewer**, kiểm tra:
   - **Hướng XT30:** miệng cắm phải quay ra mép dưới board.
   - Linh kiện cao không đụng mặt dưới DISC1. Khe hở khoảng 8.5 mm với socket chuẩn.
2. **Đo trên board DISC1 thật bằng đồng hồ:** P1-1 (góc trên, cột ngoài bên trái) phải là **5V**, P2-1 (cột trong bên phải) phải là **3V**. Shield dựa đúng vào điều này.
3. **Socket cái 2x32** phải là loại cao 8.5 mm, chân vuông 0.64 mm (lỗ 1.0 mm). Footprint dùng mẫu pad của *PinHeader* một cách có chủ ý: PinSocket của KiCad bị lật gương (chân 2 nằm ở −x), không đúng khi DISC1 chồng cùng chiều.
4. **Thứ tự chân của module GY-521 và GPS** phải khớp với nhãn in trên board: GY-521 là VCC GND SCL SDA XDA XCL AD0 INT; GPS là 5V TX RX SCL SDA GND. Module GPS khác hãng có thể khác thứ tự.
5. **Mã LCSC:** chỉ có AP63205WU-7 = C2071056 đã được xác minh. Các linh kiện khác chọn khi đặt hàng (BOM trong `fab/`).
6. **Lỗ bắt khung F450 chưa có.** Thêm sau khi đo khung.

## Firmware

Không cần sửa firmware:
- Các chân cũ dùng làm GND hoặc VCC giả (PG13, PC6, PF11, PE12, PF4, PF5…) không nối gì trên shield.
- MPU lấy nguồn 3V thật của board.
- Đo áp pin (PC3) và còi (PD4) cần **thêm code**. Phần này chưa làm.
