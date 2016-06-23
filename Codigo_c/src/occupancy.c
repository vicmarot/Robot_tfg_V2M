/*
 * File: occupancy.c
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 14-May-2016 19:20:35
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "occupancy.h"

/* Function Declarations */
static double rt_atan2d_snf(double u0, double u1);

/* Function Definitions */

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_atan2d_snf(double u0, double u1)
{
  double y;
  int b_u0;
  int b_u1;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      b_u0 = 1;
    } else {
      b_u0 = -1;
    }

    if (u1 > 0.0) {
      b_u1 = 1;
    } else {
      b_u1 = -1;
    }

    y = atan2(b_u0, b_u1);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(double)(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

/*
 * global DEBUG
 * Arguments    : const double xt[3]
 *                double Zt
 *                const double lold[250000]
 *                double lnew[250000]
 * Return Type  : void
 */
void occupancy(const double xt[3], double Zt, const double lold[250000], double
               lnew[250000])
{
  double x;
  double y;
  double theta;
  int rt;
  double b_rt;
  int i0;
  int Zeta;
  boolean_T exitg1;
  double b_Zeta;
  double Xc;
  double Yc;
  double a;
  double b_a;
  double rango;
  double angulo;
  double ln;
  double d0;

  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  /*                                                                     % */
  /*                                                                     % */
  /*            Victor Manuel Maroto                                     % */
  /*                                                                     % */
  /*        Algorithm occupancy grid mapping                             % */
  /*                                                                     % */
  /*        las matices se leen fila columna                             % */
  /*                                                                     % */
  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  x = xt[0];
  y = xt[1];
  theta = xt[2];

  /* beta=deg2rad(15);   %15*pi/180;  */
  memcpy(&lnew[0], &lold[0], 250000U * sizeof(double));

  /*      for rt= 0 : Ztmax*100 % por 100 para convertirlo en casillas.  */
  for (rt = 0; rt < 201; rt++) {
    b_rt = 100.0 + (double)rt * -0.5;

    /*  por 100 para convertirlo en casillas.  */
    i0 = (int)(((theta + 0.1308996938995747) + (0.001 - (theta -
      0.1308996938995747))) / 0.001);
    Zeta = 0;
    exitg1 = false;
    while ((!exitg1) && (Zeta <= i0 - 1)) {
      b_Zeta = (theta - 0.1308996938995747) + (double)Zeta * 0.001;
      Xc = ceil(x + b_rt * cos(b_Zeta));

      /* Xoc=ceil(x+Zt*cos(Zeta)); %celda final */
      Yc = ceil(y - b_rt * sin(b_Zeta));

      /* Yoc=ceil(y-Zt*sin(Zeta));% celda final */
      if ((Xc > 0.0) && (Yc > 0.0) && (Xc < 500.0) && (Yc < 500.0)) {
        /* % Si esta dentro del mapa */
        a = Xc - x;
        b_a = Yc - y;
        rango = sqrt(a * a + b_a * b_a);
        angulo = rt_atan2d_snf(Xc - x, Yc - y);
        if ((rango <= 100.0) && ((theta - 0.1308996938995747 < angulo) ||
             (angulo < theta + 0.1308996938995747))) {
          /*   */
          /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
          /*                                                                     % */
          /*                                                                     % */
          /*                    Victor Manuel Maroto                             % */
          /*                                                                     % */
          /*             Algorithm inverse range sensor                          % */
          /*                                                                     % */
          /*                                                                     % */
          /*      Este algoritmo pretende diferenciar cuando una casilla esta    % */
          /*                ocupada o libre                                      % */
          /*                                                                     % */
          /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
          /*  el maximo rango de Z en metros pongo un metro como maximo teorico */
          /*      alpha=55;% 2cm es el ancho del objeto error que puedo cometer 55 */
          /*  10 cm de ancho */
          /*  probabilidad alta de estar ocupado */
          /*  probabilidad baja de que este ocupado */
          /*  zmax es el maximo del haz, beta es el */
          /*  angulo de apertura del cono, alpha es el ancho del cono en esa region */
          /*  y zt es la medida realizada */
          /*      if ((Zt<Zmax )&& abs(r-Zmax)<alpha/2) % r-Zmax es la region que estara ocupada */
          if ((Zt < 100.0) && (fabs(rango - 100.0) < 1.0)) {
            /*  r-Zmax es la region que estara ocupada */
            /*          l=locc;  */
            ln = lold[(int)((Yc - 1.0) + 500.0 * (Xc - 1.0))];
          } else {
            if ((100.0 <= Zt + 1.0) || rtIsNaN(Zt + 1.0)) {
              d0 = 100.0;
            } else {
              d0 = Zt + 1.0;
            }

            if (rango > d0) {
              /*          l=lold; */
              ln = 0.8;
            } else {
              ln = 0.0;
            }
          }

          /*  l debe ser una matriz de n*m donde que reppresentara el mapa celdeado */
          /*   if ((Zt<Zmax ) && abs(r-Zmax)<alpha/2) % r-Zmax es la region que estara ocupada */
          /*          l=locc;  */
          /*   else */
          /*          l=lfree; */
          /*   end */
          /* (Zt,rango); */
          lnew[(int)((Yc - 1.0) + 500.0 * (Xc - 1.0))] = (lold[(int)((Yc - 1.0)
            + 500.0 * (Xc - 1.0))] + ln) - 0.5;
          if (lnew[(int)((Yc - 1.0) + 500.0 * (Xc - 1.0))] < 0.0) {
            lnew[(int)((Yc - 1.0) + 500.0 * (Xc - 1.0))] = 0.0;
          } else {
            if (lnew[(int)((Yc - 1.0) + 500.0 * (Xc - 1.0))] > 1.0) {
              lnew[(int)((Yc - 1.0) + 500.0 * (Xc - 1.0))] = 1.0;
            }
          }
        } else {
          lnew[(int)((Yc - 1.0) + 500.0 * (Xc - 1.0))] = lold[(int)((Yc - 1.0) +
            500.0 * (Xc - 1.0))];
        }

        Zeta++;
      } else {
        exitg1 = true;
      }
    }
  }
}

/*
 * File trailer for occupancy.c
 *
 * [EOF]
 */
