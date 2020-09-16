#include "../../../intrinsics/vima/vima.hpp"
#include <math.h>
#include <omp.h>

__v32s main(__v32s argc, char const *argv[]) {
    __v32s size = atoi(argv[1]);
    __v32s VECTOR_SIZE = atoi (argv[2]);

    if (size != 0 && (size & (size - 1)) == 0) {
        __v32s m_size = sqrt((1024 * 1024 * size) / sizeof(__v32f));
        while (m_size % 128 != 0) m_size++;
        __v32s n_vectors = ceil(((double)m_size/(double)VECTOR_SIZE));
        __v32f *matrix_a = (__v32f *)malloc(sizeof(__v32f) * m_size * (VECTOR_SIZE * n_vectors));
        __v32f *matrix_b = (__v32f *)malloc(sizeof(__v32f) * m_size * (VECTOR_SIZE * n_vectors));
        __v32f *matrix_c = (__v32f *)malloc(sizeof(__v32f) * m_size * (VECTOR_SIZE * n_vectors));
        __v32f *aux_vec = (__v32f *)malloc(sizeof(__v32f) * VECTOR_SIZE * n_vectors);
        __v32f *partial_sum = (__v32f *)malloc(sizeof(__v32f) * VECTOR_SIZE * n_vectors);

        __v32s i, j, k;

        #pragma omp parallel shared (matrix_a, matrix_b, matrix_c) private (i, j, k, partial_sum)
        {
            if (VECTOR_SIZE == 64){
                #pragma omp for schedule (static)
                for (i = 0; i < m_size; ++i) {
                    for (j = 0; j < m_size; ++j) {
                        for (k = 0; k < n_vectors; ++k) {
                            _vim64_fmuls(&matrix_a[(i * VECTOR_SIZE * n_vectors) + (k * VECTOR_SIZE)], &matrix_b[(j * VECTOR_SIZE * n_vectors) + (k * VECTOR_SIZE)], &aux_vec[k * VECTOR_SIZE]);
                            _vim64_fcums(&aux_vec[k * VECTOR_SIZE], &partial_sum[k]);
                        }
                        _vim64_fcums(partial_sum, &matrix_c[(i * m_size) + j]);
                    }
                }
            } else if (VECTOR_SIZE == 2048){
                #pragma omp for schedule (static)
                for (i = 0; i < m_size; ++i) {
                    for (j = 0; j < m_size; ++j) {
                        for (k = 0; k < n_vectors; ++k) {
                            _vim2K_fmuls(&matrix_a[(i * VECTOR_SIZE * n_vectors) + (k * VECTOR_SIZE)], &matrix_b[(j * VECTOR_SIZE * n_vectors) + (k * VECTOR_SIZE)], &aux_vec[k * VECTOR_SIZE]);
                            _vim2K_fcums(&aux_vec[k * VECTOR_SIZE], &partial_sum[k]);
                        }
                        _vim2K_fcums(partial_sum, &matrix_c[(i * m_size) + j]);
                    }
                }
            } else printf ("VECTOR SIZE must be either 64 or 2048.\n");
        }

        printf ("%f\n", matrix_c[m_size-1]);

        free(matrix_a);
        free(matrix_b);
        free(matrix_c);
        free(aux_vec);

    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }
    return 0;
}