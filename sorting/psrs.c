#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define dtype int
#define SIZE 10

#define BLOCK_LOW(id,p,n) ((id) * (n)) / (p)
#define BLOCK_HIGH(id,p,n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n) (BLOCK_HIGH(id,p,n) - BLOCK_LOW(id,p,n)+1)
#define BLOCK_OWNER(k,p,n) (((p)*(k+1)-1)/n)

dtype randDtype ();
int partition( int a[], int l, int r);
void quickSort( int a[], int l, int r);
void initArray (int arr[], int n);

int main (int argc, char** argv) {
    int rank, p;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand( time(NULL) );

    //int n = (SIZE / p) + 1;
    int n = BLOCK_SIZE(rank, p, SIZE);

    // Initialize array
    dtype local_arr[n];
    initArray(local_arr, n);

    // Sort locally
    quickSort(local_arr, 0, n-1);

    int i;
    printf("%d array: ", rank);
    for (i = 0; i < n; i++)
        printf("%d, ", local_arr[i]);
    printf("\n");
    fflush(stdout);

    dtype samples[p-1];
    // Select local samples
    printf("%d samples: ", rank);
    for (i = 0; i < p-1; i++)
        samples[i] = local_arr[ i * (n/(p*p)) ];

    for (i = 0; i < p-1; i++)
        printf("%d, ", samples[i]);
    printf("\n");

    MPI_Finalize();

    return 0;
}

dtype randDtype () {
    return rand() % 8196;
}

void initArray (dtype arr[], int n) {
    int i;
    for (i = 0; i < n; i++)
        arr[i] = randDtype();
}

void quickSort( dtype a[], int l, int r) {
   int j;

   if( l < r )
   {
       // divide and conquer
        j = partition( a, l, r);
       quickSort( a, l, j-1);
       quickSort( a, j+1, r);
   }
}

int partition( dtype a[], int l, int r) {
   int pivot, i, j, t;
   pivot = a[l];
   i = l; j = r+1;

   while(1)
   {
       do ++i; while( a[i] <= pivot && i <= r );
       do --j; while( a[j] > pivot );

       if( i >= j ) break;
       t = a[i]; a[i] = a[j]; a[j] = t;
   }

   t = a[l]; a[l] = a[j]; a[j] = t;
   return j;
}
