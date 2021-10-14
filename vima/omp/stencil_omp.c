#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include "../../../intrinsics/vima/vima.hpp"

int main(__v32s argc, char const *argv[]) {
    __v32u size = atoi(argv[1]);
    __v32u VECTOR_SIZE = atoi(argv[2]);
    if (size != 0 && (size & (size - 1)) == 0){
        __v32u i;
        __v32u v_size = (1024 * 1024 * size) / sizeof(__v32f);
        __v32f *vector_a = (__v32f *)malloc(sizeof(__v32f) * v_size);
        __v32f *vector_b = (__v32f *)malloc(sizeof(__v32f) * v_size);
        __v32f *mul = (__v32f *)malloc(sizeof(__v32f) * v_size);
        for (i = 0; i < v_size; i += VECTOR_SIZE) {
            _vim2K_fmovs(1, &vector_a[i]);
            _vim2K_fmovs(0, &vector_b[i]);
            _vim2K_fmovs(1, &mul[i]);
        }
        
        int elem = sqrt (v_size);
        int remainder = 0;
        for (int i = 0; i+elem+VECTOR_SIZE < v_size; i+=VECTOR_SIZE) remainder = i;

        #pragma omp parallel shared (vector_a, vector_b, remainder) private (i)
        {
            #pragma omp for schedule (static)
            if (VECTOR_SIZE == 2048){
                for (i = elem; i < remainder; i += VECTOR_SIZE) {
                    if (i+elem+VECTOR_SIZE > v_size) ;
                    _vim2K_fadds(&vector_b[i], &vector_a[i-elem], &vector_b[i]);
                    _vim2K_fadds(&vector_b[i], &vector_a[i], &vector_b[i]);
                    _vim2K_fadds(&vector_b[i], &vector_a[i-1], &vector_b[i]);
                    _vim2K_fadds(&vector_b[i], &vector_a[i+1], &vector_b[i]);
                    _vim2K_fadds(&vector_b[i], &vector_a[i+elem], &vector_b[i]);
                    _vim2K_fmuls(&vector_b[i], &mul[i], &vector_b[i]);
                }
            }

            if (VECTOR_SIZE == 64){
                for (i = elem; i < remainder; i += VECTOR_SIZE) {
                    if (i+elem+VECTOR_SIZE > v_size) ;
                    _vim64_fadds(&vector_b[i], &vector_a[i-elem], &vector_b[i]);
                    _vim64_fadds(&vector_b[i], &vector_a[i], &vector_b[i]);
                    _vim64_fadds(&vector_b[i], &vector_a[i-1], &vector_b[i]);
                    _vim64_fadds(&vector_b[i], &vector_a[i+1], &vector_b[i]);
                    _vim64_fadds(&vector_b[i], &vector_a[i+elem], &vector_b[i]);
                    _vim64_fmuls(&vector_b[i], &mul[i], &vector_b[i]);
                }
            }
        
            #pragma omp for schedule (static)
            for (int i = 0; i < elem; i++){
                if (i-elem > 0) vector_b[i] += vector_a[i-elem];
                if (i-1 > 0) vector_b[i] += vector_a[i-1];
                vector_b[i] += vector_a[i];
                vector_b[i] += vector_a[i+1];
                vector_b[i] += vector_a[i+elem];
                vector_b[i] *= mul[i];
            }

            #pragma omp for schedule (static)
            for (int i = remainder+elem+VECTOR_SIZE; i< v_size; i++){
                vector_b[i] += vector_a[i-elem];
                vector_b[i] += vector_a[i-1];
                vector_b[i] += vector_a[i];
                if (i+1 < v_size) vector_b[i] += vector_a[i+1];
                if (i+elem < v_size) vector_b[i] += vector_a[i+elem];
                vector_b[i] *= mul[i];
            }
        }

        printf ("%f\n", vector_b[v_size-1]);
    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }
    return 0;
}
