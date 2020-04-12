#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../intrinsics/vima/vima.hpp"

#define VECTOR_SIZE 2048

int main(__v32s argc, char const *argv[]) {
    __v32u size = atoi(argv[1]);
    if (size != 0 && (size & (size - 1)) == 0){
        __v32u v_size = (1024 * 1024 * size) / sizeof(__v32u);
        __v32u *vector_a = (__v32u *)malloc(sizeof(__v32u) * v_size);
        __v32u *vector_b = (__v32u *)malloc(sizeof(__v32u) * v_size);
        __v32u *div = (__v32u *)malloc(sizeof(__v32u)*VECTOR_SIZE);
        srand (time(NULL));
        for (int x = 0; x < v_size; x++){
            vector_a[x] = rand() % 10 + 1;
            vector_b[x] = rand() % 10 + 1;
        }
        for (int x = 0; x < VECTOR_SIZE; x++) div[x] = 5;
        int elem = sqrt (v_size);
        for (__v32u i = 0; i < v_size; i += VECTOR_SIZE) {
            _vim2K_iaddu(&vector_a[i], &vector_a[i+elem-1], &vector_b[i]);
            _vim2K_iaddu(&vector_b[i], &vector_a[i+elem], &vector_b[i]);
            _vim2K_iaddu(&vector_b[i], &vector_a[i+elem+1], &vector_b[i]);
            _vim2K_iaddu(&vector_b[i], &vector_a[i+elem*2], &vector_b[i]);
            _vim2K_idivu(&vector_b[i], &div[0], &vector_b[i]);
        }
        /*printf ("elem: %d\n", elem);
        for (int x = 0; x < v_size; x++){
            if (x % elem == 0) printf ("\n");
            printf ("%u ", vector_a[x]);
        }
        printf ("\n-------------------------\n");
        for (int x = 0; x < v_size; x++){
            if (x % elem == 0) printf ("\n");
            printf ("%u ", vector_b[x]);
        }*/
    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }
    return 0;
}
