/*
 * USER_Mixer.h
 *
 *  Quad-X motor mixer. Pure C, no HAL dependency.
 */

/*
 * ===========================================================================
 * OVERVIEW (USER_Mixer)
 * ===========================================================================
 *  Turns "how much throttle + how much roll / pitch / yaw torque" into four
 *  ESC pulse widths. Caller: Sys_RunControl() (USER_System.c) in the
 *  BLDCTask task, every 4 ms while ARMED. The result goes straight to
 *  Motor_WriteAllUs().
 *
 *  In : throttle_us (collective, e.g. 1400 us) and the rate-PID outputs
 *       roll / pitch / yaw (us, positive = positive FRD torque).
 *  Out: out_us[0..3] = M1..M4 pulse widths, always inside [min_us, max_us].
 *
 *  Motor layout (seen from above, nose up):
 *
 *        M1 (FL, CW)     M3 (FR, CCW)
 *                  \  ^  /
 *                   \ | /
 *                   /   \
 *                  /     \
 *        M2 (BL, CCW)    M4 (BR, CW)
 *
 *  Signs (MIX_Mx_* in USER_Config.h, positions confirmed 2026-11-22):
 *           roll  pitch  yaw
 *    M1 FL   +     +      -     roll > 0 (right side down) -> LEFT motors
 *    M2 BL   +     -      +       faster: M1, M2 get +, M3, M4 get -
 *    M3 FR   -     +      +     pitch > 0 (nose up) -> FRONT motors faster:
 *    M4 BR   -     -      -       M1, M3 get +, M2, M4 get -
 *                               yaw > 0 (nose right) -> CCW props faster
 *                               (M2, M3 get +): a CCW prop pushes the frame
 *                               the other way (CW), i.e. nose right.
 *
 *  Example: throttle 1400, roll +84, pitch 0, yaw 0
 *    M1 = 1400 + 84 = 1484, M2 = 1484, M3 = 1316, M4 = 1316 us.
 *    Total thrust stays about the same, only the left/right balance changes.
 */

#ifndef INC_USER_MIXER_H_
#define INC_USER_MIXER_H_

#include <stdint.h>

/*
 * throttle_us : collective throttle as ESC pulse (e.g. 1000..2000)
 * roll/pitch/yaw : PID outputs in us (positive = positive torque, FRD)
 * min_us/max_us  : output range while armed (idle .. max)
 * out_us[4]      : M1..M4 pulse widths
 *
 * If the demand saturates, the whole set is shifted (attitude authority is
 * preferred over exact throttle), then each output is clamped.
 *
 * Why shift instead of just clamping each motor? If one motor would need
 * 2100 us and is simply cut to 2000, the roll/pitch DIFFERENCE between the
 * motors is lost and the quad cannot correct its attitude. Shifting all four
 * down by 100 keeps the difference (the correction) and sacrifices a little
 * total thrust instead.
 */
void Mixer_QuadX(float throttle_us, float roll, float pitch, float yaw,
                 uint16_t min_us, uint16_t max_us, uint16_t out_us[4]);

#endif /* INC_USER_MIXER_H_ */
