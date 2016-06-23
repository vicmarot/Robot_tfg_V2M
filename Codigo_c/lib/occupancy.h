/*
 * File: occupancy.h
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 14-May-2016 19:20:35
 */

#ifndef __OCCUPANCY_H__
#define __OCCUPANCY_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "occupancy_types.h"

/* Function Declarations */
extern void occupancy(const double xt[3], double Zt, const double lold[250000],
                      double lnew[250000]);

#endif

/*
 * File trailer for occupancy.h
 *
 * [EOF]
 */
