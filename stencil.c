#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../intrinsics/vima/vima.hpp"

#define VECTOR_SIZE 2048

int main(__v32s argc, char const *argv[]) {
    __v32u size = atoi(argv[1]);
    if (size != 0 && (size & (size - 1)) == 0){
        __v32u v_size = (1024 * 1024 * size) / sizeof(__v32f);
        __v32f *vector_a = (__v32f *)malloc(sizeof(__v32f) * v_size);
        __v32f *vector_b = (__v32f *)malloc(sizeof(__v32f) * v_size);
        __v32f *mul = (__v32f *)malloc(sizeof(__v32f) * v_size);
        srand (time(NULL));
        /*for (int x = 0; x < v_size; x++){
            vector_a[x] = rand() % 10 + 1;
            vector_b[x] = rand() % 10 + 1;
            mul[x] = 2;
        }*/
        int elem = sqrt (v_size);
        for (__v32u i = 0; i < v_size; i += VECTOR_SIZE) {
            _vim2K_fadds(&vector_a[i], &vector_a[i+elem-1], &vector_b[i+elem]);
            _vim2K_fadds(&vector_b[i+elem], &vector_a[i+elem], &vector_b[i+elem]);
            _vim2K_fadds(&vector_b[i+elem], &vector_a[i+elem+1], &vector_b[i+elem]);
            _vim2K_fadds(&vector_b[i+elem], &vector_a[i+elem*2], &vector_b[i+elem]);
            _vim2K_fmuls(&vector_b[i+elem], &mul[i], &vector_b[i+elem]);
        }
        /*printf ("elem: %d\n", elem);
        for (int x = 0; x < v_size; x++){
            if (x % elem == 0) printf ("\n");
            printf ("%.0lf ", vector_a[x]);
        }
        printf ("\n-------------------------\n");
        for (int x = 0; x < v_size; x++){
            if (x % elem == 0) printf ("\n");
            printf ("%.0lf ", vector_b[x]);
        }*/
    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }
    return 0;
}
