#include "smm.h"
#include "common.h"
#include <stdio.h>
#include <omp.h>

void rec_matmul (int crow, int ccol,
                 int arow, int acol,
                 int brow, int bcol,
                 int l, int m, int n)
                 //float** a, float** b, float** c, int N)
{
    int lhalf[3], mhalf[3], nhalf[3];
    int i,j,k;
    DTYPE *aptr, *bptr, *cptr;

    if (m * n > THRESHOLD) {
        lhalf[0] = 0; lhalf[1] = l/2; lhalf[2] = l - l/2;
        mhalf[0] = 0; mhalf[1] = m/2; mhalf[2] = m - m/2;
        nhalf[0] = 0; nhalf[1] = n/2; nhalf[2] = n - n/2;

        //#pragma omp parallel for private(j,k)
        for (i = 0; i < 2; i++)
            for (j = 0; j < 2; j++)
                for (k = 0; k < 2; k++) {
                    /*
                    printf("mm(%d,%d,%d,%d,%d,%d,%d,%d,%d)\n", crow+lhalf[i], ccol+nhalf[j],
                        arow+lhalf[i], acol+mhalf[k],
                        brow+mhalf[k], bcol+nhalf[j],
                        lhalf[i+1], mhalf[k+1], nhalf[j+1]);
                        */
                    rec_matmul( crow+lhalf[i], ccol+nhalf[j],
                        arow+lhalf[i], acol+mhalf[k],
                        brow+mhalf[k], bcol+nhalf[j],
                        lhalf[i+1], mhalf[k+1], nhalf[j+1]);
                        //a,b,c,N);
                }
    }
    else {
        for (i = 0; i < l; i++)
            for (j = 0; j < n; j++) {
                cptr = &C[crow+i][ccol+j];
                aptr = &A[arow+i][acol];
                bptr = &B[brow][bcol+j];

                for (k = 0; k < m; k++) {
                    *cptr += *(aptr++) * *bptr;
                    bptr += SIZE;
                }
            }
    }
}

void matmul (int x, int y, int l, int m, int n) {
    int sum = 0;
    int c,d,k;

    #pragma omp parallel for private(d,k)
    for (c = x; c < l+y; c++) {
        for (d = y; d < n+x; d++) {
            for (k = 0; k < m; k++)
                sum = sum + A[c][k]*B[k][d];

            C[c][d] = sum;
            sum = 0;
        }
    }
}
