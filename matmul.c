#include "../intrinsics/vima/vima.hpp"
#include <math.h>

#define VECTOR_SIZE 2048

__v32s main(__v32s argc, char const *argv[]) {
    __v32s size = atoi(argv[1]);

    if (size != 0 && (size & (size - 1)) == 0) {
        __v32s m_size = sqrt((1024 * 1024 * size) / sizeof(__v32f));
        __v32s n_vectors = ceil(((double)m_size/(double)VECTOR_SIZE));
        __v32f *matrix_a = (__v32f *)malloc(sizeof(__v32f) * m_size * (VECTOR_SIZE * n_vectors));
        __v32f *matrix_b = (__v32f *)malloc(sizeof(__v32f) * m_size * (VECTOR_SIZE * n_vectors));
        __v32f *matrix_c = (__v32f *)malloc(sizeof(__v32f) * m_size * (VECTOR_SIZE * n_vectors));
        __v32f *aux_vec = (__v32f *)malloc(sizeof(__v32f) * VECTOR_SIZE * n_vectors);
        __v32f sum, partial_sum;

        /*for (int x = 0; x < m_size*VECTOR_SIZE*n_vectors; x++){
            matrix_a[x] = rand() % 10 + 1;
            matrix_b[x] = rand() % 10 + 1;
        }

        printf ("a: \n");
        for (int i = 0; i < m_size*VECTOR_SIZE*n_vectors; i++) {
            printf ("%.0f ", matrix_a[i]);
            //if (i != 0 && i % 512 == 0) printf ("\n");
        }
        printf ("\n\nb: \n");
        for (int i = 0; i < m_size*VECTOR_SIZE*n_vectors; i++) {
            printf ("%.0f ", matrix_b[i]);
            //if (i != 0 && i % 512 == 0) printf ("\n");
        }*/
        
        for (__v32s i = 0; i < m_size; ++i) {
            for (__v32s j = 0; j < m_size; ++j) {
                partial_sum = 0;
                sum = 0;
                for (__v32s k = 0; k < n_vectors; ++k) {
                    _vim2K_fmuls(&matrix_a[(i * VECTOR_SIZE * n_vectors) + (k * VECTOR_SIZE)], &matrix_b[(j * VECTOR_SIZE * n_vectors) + (k * VECTOR_SIZE)], &aux_vec[k * VECTOR_SIZE]);
                    _vim2K_fcums(&aux_vec[k * VECTOR_SIZE], &sum);
                    partial_sum += sum;
                    //printf ("a: %p | b: %p | aux: %p\n", &matrix_a[(i * VECTOR_SIZE * n_vectors) + (k * VECTOR_SIZE)], &matrix_b[(j * VECTOR_SIZE * n_vectors) + (k * VECTOR_SIZE)], &aux_vec[k * VECTOR_SIZE]);
                }
                matrix_c[(i * m_size) + j] = partial_sum;
                //printf ("C: %p\n", &matrix_c[(i * VECTOR_SIZE) + j]);
            }
        }

        /*printf ("\n\nb: \n");
        for (int i = 0; i < m_size*VECTOR_SIZE*n_vectors; i++) {
            printf ("%.0f ", matrix_c[i]);
            //if (i != 0 && i % 512 == 0) printf ("\n");
        }*/

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