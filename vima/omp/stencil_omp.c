#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include "../../../intrinsics/vima/vima.hpp"

#define VECTOR_SIZE 2048

int main(__v32s argc, char const *argv[]) {
    __v32u size = atoi(argv[1]);
    if (size != 0 && (size & (size - 1)) == 0){
        __v32u i;
        __v32u v_size = (1024 * 1024 * size) / sizeof(__v32f);
        __v32f *vector_a = (__v32f *)malloc(sizeof(__v32f) * v_size);
        __v32f *vector_b = (__v32f *)malloc(sizeof(__v32f) * v_size);
        __v32f *mul = (__v32f *)malloc(sizeof(__v32f) * v_size);
        for (i = 0; i < v_size; i += VECTOR_SIZE) {
            _vim2K_fmovs(1, &vector_a[i]);
            _vim2K_fmovs(1, &vector_b[i]);
            _vim2K_fmovs(1, &mul[i]);
        }
        
        int elem = sqrt (v_size);

        #pragma omp parallel shared (vector_a, vector_b) private (i)
        {
            #pragma omp for schedule (static, 1)
            for (i = 0; i < v_size; i += VECTOR_SIZE) {
                _vim2K_fadds(&vector_a[i], &vector_a[i+elem-1], &vector_b[i+elem]);
                _vim2K_fadds(&vector_b[i+elem], &vector_a[i+elem], &vector_b[i+elem]);
                _vim2K_fadds(&vector_b[i+elem], &vector_a[i+elem+1], &vector_b[i+elem]);
                _vim2K_fadds(&vector_b[i+elem], &vector_a[i+elem*2], &vector_b[i+elem]);
                _vim2K_fmuls(&vector_b[i+elem], &mul[i], &vector_b[i+elem]);
            }
        }

        printf ("%f\n", vector_b[v_size-1]);
    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }
    return 0;
}
