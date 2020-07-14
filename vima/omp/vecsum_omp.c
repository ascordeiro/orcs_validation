#include "../../../intrinsics/vima/vima.hpp"
#include <omp.h>

#define VECTOR_SIZE 2048

__v32s main(__v32s argc, char const *argv[]) {
    __v32s size = atoi(argv[1]);
    if (size != 0 && (size & (size - 1)) == 0){
        __v32u v_size = (1024 * 1024 * size) / sizeof(__v32f);
        __v32f *vector_a = (__v32f *)malloc(sizeof(__v32f) * v_size);
        __v32f *vector_b = (__v32f *)malloc(sizeof(__v32f) * v_size);
        __v32f *vector_c = (__v32f *)malloc(sizeof(__v32f) * v_size);
        __v32s i;

        #pragma omp parallel shared (vector_a, vector_b, vector_c) private (i)
        {
            #pragma omp for schedule (dynamic)
            for (i = 0; i < v_size; i += VECTOR_SIZE) {
                _vim2K_fadds(&vector_a[i], &vector_b[i], &vector_c[i]);
            }
        }
        
        printf ("%f ", vector_c[v_size-1]);

        free (vector_a);
        free (vector_b);
        free (vector_c);
    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }
    return 0;
}
