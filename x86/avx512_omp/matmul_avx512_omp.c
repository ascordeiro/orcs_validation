#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>
#include <omp.h>


int main(int argc, char const *argv[]) {
    int size = atoi(argv[1]);

    if (size != 0 && (size & (size - 1)) == 0) {
        int m_size = sqrt((1024 * 1024 * size) / sizeof(float));
        while (m_size % 256 != 0) m_size++;
        float *matrix_a = (float *) aligned_alloc(32, sizeof (float) * m_size * m_size);
        float *matrix_b = (float *) aligned_alloc(32, sizeof (float) * m_size * m_size);
        float *matrix_c = (float *) aligned_alloc(32, sizeof (float) * m_size * m_size);

        int BLOCKSIZE = 64 / sizeof (float);
        int count = 0;
        
        int i, j, k, i0, k0, j0;
        __m256 vec_a, vec_b, aux_vec;
        #pragma omp parallel shared (vec_a, vec_b, matrix_a, matrix_b, matrix_c) private (i, j, k, aux_vec)
        {
            #pragma omp for schedule (static)
            for(i0 = 0; i0 < m_size; i0 += BLOCKSIZE) {
                for(k0 = 0; k0 < m_size; k0 += BLOCKSIZE) {
                    for(j0 = 0; j0 < m_size; j0 += BLOCKSIZE) {
                        for (i = i0; i < i0+BLOCKSIZE; i++) {
                            for (j = j0; j < j0+BLOCKSIZE; j+=16)  {
                                //printf ("C: i = %d, j = %d\n", i, j);
                                aux_vec = _mm256_load_ps(&matrix_c[i*m_size + j]);
                                for (k = k0; k < k0+BLOCKSIZE; k++) {
                                    //printf ("A: i = %d, k = %d\n", i, k);
                                    vec_a = _mm256_load_ps (&matrix_a[i*m_size + k]);
                                    //printf ("\nvec_a  : ");
                                    //for (int i = 0; i < 16; i++) printf ("%.0f ", vec_a[i]);
                                    //printf ("B: k = %d, j = %d\n", k, j);
                                    vec_b = _mm256_load_ps (&matrix_b[k*m_size + j]);
                                    //printf ("\nvec_b  : ");
                                    //for (int i = 0; i < 16; i++) printf ("%.0f ", vec_b[i]);
                                    aux_vec = _mm256_add_ps (aux_vec, _mm256_mul_ps (vec_a, vec_b));
                                    count++;
                                }
                                //printf ("C: i = %d, j = %d\n", i, j);
                                _mm256_store_ps(&matrix_c[i*m_size + j], aux_vec);
                            }
                        }
                    }
                }
            }
        }

        printf ("%f\n", matrix_c[m_size*m_size-1]);
        printf ("count = %d\n", count);
    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }
    return 0;
}