
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char const *argv[]) {
    int size = atoi(argv[1]);
    int v_size = (1024 * 1024 * size)/sizeof(int);
    int elem = sqrt (v_size);
    while (elem % 16 != 0) elem++;
    printf ("%d\n", elem);
        
    int** A = (int**) malloc (sizeof (int*) * elem);
    for (int x = 0; x < elem; x++) A[x] = (int*) malloc (sizeof(int) * elem);
    int** B = (int**) malloc (sizeof (int*) * elem);
    for (int x = 0; x < elem; x++) B[x] = (int*) malloc (sizeof(int) * elem);
    int** C = (int**) malloc (sizeof (int*) * elem);
    for (int x = 0; x < elem; x++) C[x] = (int*) malloc (sizeof(int) * elem);

    for (int x = 0; x < elem; x++){
        for (int y = 0; y < elem; y++){
            A[x][y] = rand() % 2;
            B[x][y] = rand() % 2;
        }
    }

    if (size != 0 && (size & (size - 1)) == 0){
        int i, j, k, ii, ib, kb, kk, acc00, acc01, acc10, acc11;
        ib = 16;
        for (ii = 0; ii < elem; ii += ib){
            for (j = 0; j < elem; j += 2){
                for (i = ii; i < ii + ib; i += 2){
                    acc00 = acc01 = acc10 = acc11 = 0;
                    for (k = 0; k < elem; k++){
                        acc00 += B[k][j + 0] * A[i + 0][k];
                        acc01 += B[k][j + 1] * A[i + 0][k];
                        acc10 += B[k][j + 0] * A[i + 1][k];
                        acc11 += B[k][j + 1] * A[i + 1][k];
                    }
                    C[i + 0][j + 0] = acc00;
                    C[i + 0][j + 1] = acc01;
                    C[i + 1][j + 0] = acc10;
                    C[i + 1][j + 1] = acc11;
                }
            }
        }
    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }

    /*for (int x = 0; x < elem; x++){
        for (int y = 0; y < elem; y++){
            printf ("%d ", A[x][y]);
        }
        printf ("\n");
    }
    printf ("--------------------------\n");
    for (int x = 0; x < elem; x++){
        for (int y = 0; y < elem; y++){
            printf ("%d ", B[x][y]);
        }
        printf ("\n");
    }
    printf ("--------------------------\n");
    for (int x = 0; x < elem; x++){
        for (int y = 0; y < elem; y++){
            printf ("%d ", C[x][y]);
        }
        printf ("\n");
    }*/
    
    return 0;
}
