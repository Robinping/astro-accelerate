//Added by Karel Adamek
#include "headers/device_MSD_Configuration.h"

#ifndef __MSD_BLN_PW__
#define __MSD_BLN_PW__

extern void MSD_BLN_pw_init(void);

extern int MSD_BLN_pw(float *d_input, float *d_MSD, int nDMs, int nTimesamples, int offset, float multiplier);

extern int MSD_BLN_pw(float *d_input, float *d_MSD, float *d_temp, MSD_Configuration *MSD_conf, float bln_sigma_constant);

extern int MSD_BLN_pw_continuous(float *d_input, float *d_MSD, float *d_previous_partials, float *d_temp, MSD_Configuration *MSD_conf, float bln_sigma_constant);

extern int MSD_BLN_pw_continuous_OR(float *d_input, float *d_MSD, float *d_previous_partials, float *d_temp, MSD_Configuration *MSD_conf, float bln_sigma_constant);

extern int MSD_BLN_LA_pw_normal(float *d_input, float *d_MSD_T, int nTaps, int nDMs, int nTimesamples, int offset, float bln_sigma_constant);

extern int MSD_BLN_LA_Nth_pw_normal(float *d_input, float *d_bv_in, float *d_MSD_T, float *d_MSD_DIT, int nTaps, int nDMs, int nTimesamples, int offset, int DIT_value, float bln_sigma_constant);

#endif
