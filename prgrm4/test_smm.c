#include "smm.h"
#include <stdio.h>
#include <sys/time.h>
#include <malloc.h>
#include "common.h"

void alloc_mat (float*** A, int size) {
    float* Astorage;

    // Allocate space
    Astorage = (float*) malloc(size*size * sizeof(float));
    if (Astorage == NULL) {
        printf("Astorage mem could not allocate\n");
        exit(0);
    }

    *A = (float**) malloc(size * sizeof(float*));
    if (A == NULL) {
        printf("A mem could not allocate\n");
        exit(0);
    }

    int i;
    for (i = 0; i < size; i++) {
        (*A)[i] = &Astorage[i * size];
    }

    // TODO: Need to free Astorage
}

void gen_submats (int size, int start_coords[2]) {
    int i,j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++) {
            if (i == j - 1 || j == i - 1) {
                A[i][j] = 1.;
                B[i][j] = 1.;
            }
            else {
                A[i][j] = 0.;
                B[i][j] = 0.;
            }
            /*
            A[i][j] = j-i;
            B[i][j] = SIZE-j+i;
            */
            C[i][j] = 0.;
        }
}

void printmat (DTYPE** mat, int n) {
    fflush(stdout);
    int i,j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%7.0f",mat[i][j]);
        putchar('\n');
    }
}

int verify_result() {
    int i,j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            if (C[i][j] != SIZE)
                return 0;
    return 1;
}

int main(int argc, char** argv) {
    alloc_mat(&A, SIZE);
    alloc_mat(&B, SIZE);
    alloc_mat(&C, SIZE);

    int coords[2] = {0,0};
    gen_submats(SIZE, coords);
    /*
    printf("A\n-------------\n");
    printmat(A, SIZE);
    printf("B\n-------------\n");
    printmat(B, SIZE);
    */

    struct timeval start_time, stop_time, elapsed_time;

    //-----------
    // Start time
    //-----------
    gettimeofday(&start_time,NULL);

    rec_matmul(0,0,0,0,0,0,SIZE,SIZE,SIZE);
    //matmul(0,0,SIZE,SIZE,SIZE);

    //---------
    // End time
    //---------
    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine

    //printf("C\n-------------\n");
    //printmat(C, SIZE);

    float GFLOPS = (float)(2.f*SIZE*SIZE*SIZE) / (1000000000.f*(elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0));
    printf("elapsed time (s): %f\n", ((elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0)));
    printf("GFLOPS: %f\n", GFLOPS);

    return 0;
}
