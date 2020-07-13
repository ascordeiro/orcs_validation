#include "../../../intrinsics/vima/vima.hpp"
#include <omp.h>

#define VECTOR_SIZE 2048

__v32s main(__v32s argc, char const *argv[]) {
    __v32s size = atoi(argv[1]);
    if (size != 0 && (size & (size - 1)) == 0){
        __v32s v_size = (1024 * 1024 * size) / sizeof(__v32s);
        __v32s *vector = (__v32s *)malloc(sizeof(__v32s) * v_size);
        __v32s i;
        int tid, start, finish;
        #pragma omp parallel shared (vector) private (i, tid, start, finish)
        {
            int chunk_size = v_size / omp_get_num_threads();
            tid = omp_get_thread_num();
            start = tid*chunk_size;
            finish = start + chunk_size;
            #pragma omp for schedule (dynamic)
            for (i = start; i < finish; i += VECTOR_SIZE) {
                _vim2K_imovs(0, &vector[i]);
            }
        }

        printf ("%d\n", vector[v_size-1]);
        
        free (vector);
    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }
    return 0;
}
