#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../intrinsics/vima/vima.hpp"

int main(__v32s argc, char const *argv[]) {
    __v32s size = atoi(argv[1]);
    __v32s VECTOR_SIZE = atoi (argv[2]);

    if (size != 0 && (size & (size - 1)) == 0){
        __v32u v_size = (size * size);
        __v32f *vector_a, *vector_b, *mul;
        if (v_size >= VECTOR_SIZE) {
            vector_a = (__v32f *) aligned_alloc(256, sizeof(__v32f) * v_size);
            vector_b = (__v32f *) aligned_alloc(256, sizeof(__v32f) * v_size);
            mul = (__v32f *)malloc(sizeof(__v32f) * v_size);
        } else {
            vector_a = (__v32f *) aligned_alloc(256, sizeof(__v32f) * v_size);
            vector_b = (__v32f *) aligned_alloc(256, sizeof(__v32f) * v_size);
            mul = (__v32f *)malloc(sizeof(__v32f) * v_size);
        }
        srand (time(NULL));
        int elem = size;
        if (VECTOR_SIZE == 2048){
            for (int j = 0; j < 256; j++){
                for (__v32u i = elem; i < v_size; i += VECTOR_SIZE) {
                    _vim2K_fadds(&vector_a[i], &vector_a[i-elem], &vector_b[i]);
                    _vim2K_fadds(&vector_b[i], &vector_a[i+elem], &vector_b[i]);
                    _vim2K_fadds(&vector_b[i], &vector_a[i+1], &vector_b[i]);
                    _vim2K_fadds(&vector_b[i], &vector_a[i-1], &vector_b[i]);
                    _vim2K_fmuls(&vector_b[i], &mul[i], &vector_b[i]);
                }
            }
        } else if (VECTOR_SIZE == 64){
            for (int j = 0; j < 256; j++){
                for (__v32u i = elem; i < v_size; i += VECTOR_SIZE) {
                    _vim64_fadds(&vector_a[i-elem], &vector_a[i-1], &vector_b[i]);
                    _vim64_fadds(&vector_b[i], &vector_a[i], &vector_b[i]);
                    _vim64_fadds(&vector_b[i], &vector_a[i+1], &vector_b[i]);
                    _vim64_fadds(&vector_b[i], &vector_a[i+elem], &vector_b[i]);
                    _vim64_fmuls(&vector_b[i], &mul[i], &vector_b[i]);
                }
            }
        } else printf ("O vetor deve ter tamanho 2048 ou 64!\n");
        printf ("%0.lf\n ", vector_b[v_size-1]);
    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }
    return 0;
}
