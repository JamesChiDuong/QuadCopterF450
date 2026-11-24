/*
 * USER_Mixer.c
 *
 *  See USER_Mixer.h. Signs come from USER_Config.h (MIX_Mx_*).
 */

/*
 * ===========================================================================
 * OVERVIEW (USER_Mixer.c)
 * ===========================================================================
 *  Quad-X mixer (read the OVERVIEW and motor diagram in USER_Mixer.h first).
 *  Called by Sys_RunControl() every 4 ms while ARMED (also once per cycle on
 *  the ground with zero torques, which just gives MOTOR_IDLE_US on all four).
 *
 *  Steps in Mixer_QuadX():
 *    1. m[i] = throttle + sign_roll*roll + sign_pitch*pitch + sign_yaw*yaw
 *    2. find the highest and lowest motor
 *    3. if the highest is above max_us, shift ALL motors down
 *    4. if the lowest is (still) below min_us, shift ALL motors up
 *       (step 4 wins over step 3: keeping every motor above idle matters more,
 *        a stopped motor in the air = no control at all)
 *    5. clamp each motor to [min_us, max_us] and round to integer us
 */
#include "USER_Mixer.h"
#include "USER_Config.h"

/* Row = motor M1..M4, column = sign for roll, pitch, yaw (+1 or -1) */
static const float s_mix[4][3] =
{
    /*   roll           pitch           yaw        */
    { MIX_M1_ROLL, MIX_M1_PITCH, MIX_M1_YAW },
    { MIX_M2_ROLL, MIX_M2_PITCH, MIX_M2_YAW },
    { MIX_M3_ROLL, MIX_M3_PITCH, MIX_M3_YAW },
    { MIX_M4_ROLL, MIX_M4_PITCH, MIX_M4_YAW },
};

/*
 * Mix throttle + torques into 4 pulse widths (all values in us).
 *   min_us / max_us : allowed range (MOTOR_IDLE_US .. MOTOR_MAX_US)
 *   out_us[4]       : result, M1..M4
 * Example of the shift: throttle 1850, pitch +200 -> front motors want 2050,
 * rear 1650. Highest = 2050 > 2000, so all go down by 50: front 2000,
 * rear 1600. The 400 us front/rear difference (the correction) is kept.
 */
void Mixer_QuadX(float throttle_us, float roll, float pitch, float yaw,
                 uint16_t min_us, uint16_t max_us, uint16_t out_us[4])
{
    float m[4];
    float mx, mn;
    int i;

    for (i = 0; i < 4; i++)
    {
        m[i] = throttle_us + s_mix[i][0] * roll + s_mix[i][1] * pitch +
               s_mix[i][2] * yaw;
    }

    mx = m[0];
    mn = m[0];
    for (i = 1; i < 4; i++)
    {
        if (m[i] > mx) mx = m[i];
        if (m[i] < mn) mn = m[i];
    }

    /* Shift down if above max, then up if below min */
    if (mx > (float)max_us)
    {
        for (i = 0; i < 4; i++)
            m[i] -= (mx - (float)max_us);
        mn -= (mx - (float)max_us);     /* keep mn in sync for the next test */
    }
    if (mn < (float)min_us)
    {
        for (i = 0; i < 4; i++)
            m[i] += ((float)min_us - mn);
    }

    /* Final clamp: only needed when the torque spread is wider than
     * max_us - min_us (then the shift alone cannot fit it). +0.5 = rounding. */
    for (i = 0; i < 4; i++)
    {
        if (m[i] < (float)min_us)
            m[i] = (float)min_us;
        if (m[i] > (float)max_us)
            m[i] = (float)max_us;
        out_us[i] = (uint16_t)(m[i] + 0.5f);
    }
}
