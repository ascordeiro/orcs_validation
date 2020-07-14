#include "../../../intrinsics/vima/vima.hpp"
#include <omp.h>

#define VECTOR_SIZE 2048

__v32s main(__v32s argc, char const *argv[]) {
    __v32s size = atoi(argv[1]);
    if (size != 0 && (size & (size - 1)) == 0){
        __v32s v_size = (1024 * 1024 * size) / sizeof(__v32s);
        __v32s *vector_a = (__v32s *)malloc(sizeof(__v32s) * v_size);
        __v32s *vector_b = (__v32s *)malloc(sizeof(__v32s) * v_size);
        __v32s i;

        #pragma omp parallel shared (vector_a, vector_b) private (i)
        {
            #pragma omp for schedule (dynamic)
            for (i = 0; i < v_size; i += VECTOR_SIZE) {
                _vim2K_icpys(&vector_a[i], &vector_b[i]);
            }
        }

        printf ("%d\n", vector_b[v_size-1]);

        free (vector_a);
        free (vector_b);
    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }
    return 0;
}
