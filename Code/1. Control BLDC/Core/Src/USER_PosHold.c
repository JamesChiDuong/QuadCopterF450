/*
 * USER_PosHold.c
 *
 *  GPS position hold: position P -> speed PI -> tilt (see USER_PosHold.h).
 *  Pure C; mirrored in host/fc_model.py (class PosHold) - keep them equal.
 */

#include "USER_PosHold.h"
#include <math.h>

#define PH_DEG2RAD  0.01745329252f

static float PH_Clamp(float v, float lim)
{
    if (v > lim)
        return lim;
    if (v < -lim)
        return -lim;
    return v;
}

void PosHold_Start(PosHold_TypeDef *ph, float pos_n, float pos_e)
{
    ph->target_n = pos_n;
    ph->target_e = pos_e;
    ph->i_n = 0.0f;
    ph->i_e = 0.0f;
    ph->active = true;
}

void PosHold_Stop(PosHold_TypeDef *ph)
{
    ph->active = false;
    ph->i_n = 0.0f;
    ph->i_e = 0.0f;
}

void PosHold_FollowPilot(PosHold_TypeDef *ph, float pos_n, float pos_e)
{
    ph->target_n = pos_n;
    ph->target_e = pos_e;
}

void PosHold_Update(PosHold_TypeDef *ph, const PosHold_Gains *g,
                    float pos_n, float pos_e, float vel_n, float vel_e,
                    float heading_deg, float dt,
                    float *roll_sp_deg, float *pitch_sp_deg)
{
    float sp_vn, sp_ve, len, err_vn, err_ve, tilt_n, tilt_e, fwd, right;
    float c, s;

    if (!ph->active)
    {
        *roll_sp_deg = 0.0f;
        *pitch_sp_deg = 0.0f;
        return;
    }

    /* 1) Position error -> wanted ground speed, limited as a VECTOR so the
     *    direction is kept (limiting N and E separately would bend the path) */
    sp_vn = (ph->target_n - pos_n) * g->pos_p;
    sp_ve = (ph->target_e - pos_e) * g->pos_p;
    len = sqrtf(sp_vn * sp_vn + sp_ve * sp_ve);
    if (len > g->max_speed)
    {
        sp_vn *= g->max_speed / len;
        sp_ve *= g->max_speed / len;
    }

    /* 2) Speed error -> wanted tilt towards North / East (PI). The I term
     *    learns the steady tilt needed against wind. */
    err_vn = sp_vn - vel_n;
    err_ve = sp_ve - vel_e;
    ph->i_n = PH_Clamp(ph->i_n + err_vn * g->vel_i * dt, g->vel_i_limit);
    ph->i_e = PH_Clamp(ph->i_e + err_ve * g->vel_i * dt, g->vel_i_limit);
    tilt_n = err_vn * g->vel_p + ph->i_n;
    tilt_e = err_ve * g->vel_p + ph->i_e;

    /* 3) Earth (N, E) -> body (forward, right) with the heading psi:
     *    forward =  cos(psi) N + sin(psi) E
     *    right   = -sin(psi) N + cos(psi) E */
    c = cosf(heading_deg * PH_DEG2RAD);
    s = sinf(heading_deg * PH_DEG2RAD);
    fwd = c * tilt_n + s * tilt_e;
    right = -s * tilt_n + c * tilt_e;

    /* 4) Tilt -> angle setpoints (FRD): forward needs nose DOWN (pitch < 0),
     *    right needs right side down (roll > 0). Limit the total tilt. */
    len = sqrtf(fwd * fwd + right * right);
    if (len > g->max_angle)
    {
        fwd *= g->max_angle / len;
        right *= g->max_angle / len;
    }
    *pitch_sp_deg = -fwd;
    *roll_sp_deg = right;
}
