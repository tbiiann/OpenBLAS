/***************************************************************************
Copyright (c) 2013-2017, The OpenBLAS Project
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in
the documentation and/or other materials provided with the
distribution.
3. Neither the name of the OpenBLAS project nor the names of
its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE OPENBLAS PROJECT OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "common.h"
#include <math.h>

#if defined(DOUBLE)
#define ABS fabs
#else
#define ABS fabsf
#endif

#define CABS1(x,i) (ABS(x[i]) + ABS(x[i + 1]))

static FLOAT camax_kernel_32(BLASLONG n, FLOAT *x)
{
    FLOAT amax;

    __asm__ volatile (
        "vlef   %%v0,0(%2),0              \n\t"
        "vlef   %%v16,4(%2),0             \n\t"
        "vlef   %%v0,8(%2),1              \n\t"
        "vlef   %%v16,12(%2),1            \n\t"
        "vlef   %%v0,16(%2),2             \n\t"
        "vlef   %%v16,20(%2),2            \n\t"
        "vlef   %%v0,24(%2),3             \n\t"
        "vlef   %%v16,28(%2),3            \n\t"
        "vflpsb %%v0,%%v0                 \n\t"
        "vflpsb %%v16,%%v16               \n\t"
        "vfasb  %%v0,%%v0,%%v16           \n\t"
        "srlg  %%r0,%1,5                  \n\t"
        "xgr %%r1,%%r1                    \n\t"
        "0:                               \n\t"
        "pfd 1, 1024(%%r1,%2)             \n\t"

        "vlef  %%v16,0(%%r1,%2),0         \n\t"
        "vlef  %%v17,4(%%r1,%2),0         \n\t"
        "vlef  %%v16,8(%%r1,%2),1         \n\t"
        "vlef  %%v17,12(%%r1,%2),1        \n\t"
        "vlef  %%v16,16(%%r1,%2),2        \n\t"
        "vlef  %%v17,20(%%r1,%2),2        \n\t"
        "vlef  %%v16,24(%%r1,%2),3        \n\t"
        "vlef  %%v17,28(%%r1,%2),3        \n\t"

        "vlef  %%v18,32(%%r1,%2),0        \n\t"
        "vlef  %%v19,36(%%r1,%2),0        \n\t"
        "vlef  %%v18,40(%%r1,%2),1        \n\t"
        "vlef  %%v19,44(%%r1,%2),1        \n\t"
        "vlef  %%v18,48(%%r1,%2),2        \n\t"
        "vlef  %%v19,52(%%r1,%2),2        \n\t"
        "vlef  %%v18,56(%%r1,%2),3        \n\t"
        "vlef  %%v19,30(%%r1,%2),3        \n\t"

        "vlef  %%v20,64(%%r1,%2),0        \n\t"
        "vlef  %%v21,68(%%r1,%2),0        \n\t"
        "vlef  %%v20,72(%%r1,%2),1        \n\t"
        "vlef  %%v21,76(%%r1,%2),1        \n\t"
        "vlef  %%v20,80(%%r1,%2),2        \n\t"
        "vlef  %%v21,84(%%r1,%2),2        \n\t"
        "vlef  %%v20,88(%%r1,%2),3        \n\t"
        "vlef  %%v21,92(%%r1,%2),3        \n\t"

        "vlef  %%v22,96(%%r1,%2),0        \n\t"
        "vlef  %%v23,100(%%r1,%2),0       \n\t"
        "vlef  %%v22,104(%%r1,%2),1       \n\t"
        "vlef  %%v23,108(%%r1,%2),1       \n\t"
        "vlef  %%v22,112(%%r1,%2),2       \n\t"
        "vlef  %%v23,116(%%r1,%2),2       \n\t"
        "vlef  %%v22,120(%%r1,%2),3       \n\t"
        "vlef  %%v23,124(%%r1,%2),3       \n\t"

        "vlef  %%v24,128(%%r1,%2),0       \n\t"
        "vlef  %%v25,132(%%r1,%2),0       \n\t"
        "vlef  %%v24,136(%%r1,%2),1       \n\t"
        "vlef  %%v25,140(%%r1,%2),1       \n\t"
        "vlef  %%v24,144(%%r1,%2),2       \n\t"
        "vlef  %%v25,148(%%r1,%2),2       \n\t"
        "vlef  %%v24,152(%%r1,%2),3       \n\t"
        "vlef  %%v25,156(%%r1,%2),3       \n\t"

        "vlef  %%v26,160(%%r1,%2),0       \n\t"
        "vlef  %%v27,164(%%r1,%2),0       \n\t"
        "vlef  %%v26,168(%%r1,%2),1       \n\t"
        "vlef  %%v27,172(%%r1,%2),1       \n\t"
        "vlef  %%v26,176(%%r1,%2),2       \n\t"
        "vlef  %%v27,180(%%r1,%2),2       \n\t"
        "vlef  %%v26,184(%%r1,%2),3       \n\t"
        "vlef  %%v27,188(%%r1,%2),3       \n\t"

        "vlef  %%v28,192(%%r1,%2),0       \n\t"
        "vlef  %%v29,196(%%r1,%2),0       \n\t"
        "vlef  %%v28,200(%%r1,%2),1       \n\t"
        "vlef  %%v29,204(%%r1,%2),1       \n\t"
        "vlef  %%v28,208(%%r1,%2),2       \n\t"
        "vlef  %%v29,212(%%r1,%2),2       \n\t"
        "vlef  %%v28,216(%%r1,%2),3       \n\t"
        "vlef  %%v29,220(%%r1,%2),3       \n\t"

        "vlef  %%v30,224(%%r1,%2),0       \n\t"
        "vlef  %%v31,228(%%r1,%2),0       \n\t"
        "vlef  %%v30,232(%%r1,%2),1       \n\t"
        "vlef  %%v31,236(%%r1,%2),1       \n\t"
        "vlef  %%v30,240(%%r1,%2),2       \n\t"
        "vlef  %%v31,244(%%r1,%2),2       \n\t"
        "vlef  %%v30,248(%%r1,%2),3       \n\t"
        "vlef  %%v31,252(%%r1,%2),3       \n\t"

        "vflpsb  %%v16,%%v16              \n\t"
        "vflpsb  %%v17,%%v17              \n\t"
        "vflpsb  %%v18,%%v18              \n\t"
        "vflpsb  %%v19,%%v19              \n\t"
        "vflpsb  %%v20,%%v20              \n\t"
        "vflpsb  %%v21,%%v21              \n\t"
        "vflpsb  %%v22,%%v22              \n\t"
        "vflpsb  %%v23,%%v23              \n\t"
        "vflpsb  %%v24,%%v24              \n\t"
        "vflpsb  %%v25,%%v25              \n\t"
        "vflpsb  %%v26,%%v26              \n\t"
        "vflpsb  %%v27,%%v27              \n\t"
        "vflpsb  %%v28,%%v28              \n\t"
        "vflpsb  %%v29,%%v29              \n\t"
        "vflpsb  %%v30,%%v30              \n\t"
        "vflpsb  %%v31,%%v31              \n\t"

        "vfasb %%v16,%%v16,%%v17          \n\t"
        "vfasb %%v18,%%v18,%%v19          \n\t"
        "vfasb %%v20,%%v20,%%v21          \n\t"
        "vfasb %%v22,%%v22,%%v23          \n\t"
        "vfasb %%v24,%%v24,%%v25          \n\t"
        "vfasb %%v26,%%v26,%%v27          \n\t"
        "vfasb %%v28,%%v28,%%v29          \n\t"
        "vfasb %%v30,%%v30,%%v31          \n\t"
        
        "vfmaxsb  %%v16,%%v16,%%v24,0     \n\t"
        "vfmaxsb  %%v18,%%v18,%%v26,0     \n\t"
        "vfmaxsb  %%v20,%%v20,%%v28,0     \n\t"
        "vfmaxsb  %%v22,%%v22,%%v30,0     \n\t"

        "vfmaxsb  %%v16,%%v16,%%v20,0     \n\t"
        "vfmaxsb  %%v18,%%v18,%%v22,0     \n\t"

        "vfmaxsb  %%v16,%%v16,%%v18,0     \n\t"

        "vfmaxsb  %%v0,%%v0,%%v16,0       \n\t"

        "agfi    %%r1, 256                \n\t"
        "brctg   %%r0, 0b                 \n\t"

        "veslg   %%v16,%%v0,32            \n\t"
        "vfmaxsb %%v0,%%v0,%%v16,0        \n\t"

        "vrepf  %%v16,%%v0,2              \n\t"
        "wfmaxsb %%v0,%%v0,%%v16,0        \n\t"
        "ler    %0,%%f0                       "
        :"=f"(amax)
        :"r"(n),"ZR"((const FLOAT (*)[n])x)
        :"memory","cc","r0","r1","v0","v16","v17","v18","v19","v20","v21","v22","v23","v24","v25","v26","v27"
    );

    return amax;
}
 
FLOAT CNAME(BLASLONG n, FLOAT *x, BLASLONG inc_x) {
    BLASLONG i = 0;
    BLASLONG ix = 0;
    FLOAT maxf = 0.0;
    BLASLONG inc_x2;

    if (n <= 0 || inc_x <= 0) return (maxf);

    if (inc_x == 1) {

        BLASLONG n1 = n & -32;
        if (n1 > 0) {

            maxf = camax_kernel_32(n1, x);

            i = n1;
        }
        else
        {
            maxf=CABS1(x,0);
            ix += 2;
            i++;
        }

        while (i < n) {
            if (CABS1(x,ix) > maxf) {
                maxf = CABS1(x,ix);
            }
            ix += 2;
            i++;
        }
        return (maxf);

    } else {

        maxf=CABS1(x,0);
        inc_x2 = 2 * inc_x;

        BLASLONG n1 = n & -4;
        while (i < n1) {

            if (CABS1(x,ix) > maxf) {
                maxf = CABS1(x,ix);
            }
            if (CABS1(x,ix+inc_x2) > maxf) {
                maxf = CABS1(x,ix+inc_x2);
            }
            if (CABS1(x,ix+inc_x2*2) > maxf) {
                maxf = CABS1(x,ix+inc_x2*2);
            }
            if (CABS1(x,ix+inc_x2*3) > maxf) {
                maxf = CABS1(x,ix+inc_x2*3);
            }

            ix += inc_x2 * 4;

            i += 4;

        }


        while (i < n) {
            if (CABS1(x,ix) > maxf) {
                maxf = CABS1(x,ix);
            }
            ix += inc_x2;
            i++;
        }
        return (maxf);
    }
}
