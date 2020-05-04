#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>

int main(int argc, char const *argv[]) {
    int size = atoi(argv[1]);

    if (size != 0 && (size & (size - 1)) == 0) {
        int m_size = sqrt((1024 * 1024 * size) / sizeof(float));
        while (m_size % 256 != 0) m_size++;
        int n_vectors = ceil(((double) m_size/(double) 16));
        float *matrix_a = (float *) aligned_alloc(32, sizeof (float) * m_size * (16 * n_vectors));
        float *matrix_b = (float *) aligned_alloc(32, sizeof (float) * m_size * (16 * n_vectors));
        float *matrix_c = (float *) aligned_alloc(32, sizeof (float) * m_size * (16 * n_vectors));
        float sum, partial_sum;

        /*printf ("%d \n", m_size);

        for (int x = 0; x < m_size*16*n_vectors; x++){
            matrix_a[x] = rand() % 10 + 1;
            matrix_b[x] = rand() % 10 + 1;
        }*/

        /*printf ("a: \n");
        for (int i = 0; i < m_size*16*n_vectors; i++) {
            if (i != 0 && i % m_size == 0) printf ("\n");
            printf ("%.0f ", matrix_a[i]);
        }
        printf ("\n\nb: \n");
        for (int i = 0; i < m_size*16*n_vectors; i++) {
            if (i != 0 && i % m_size == 0) printf ("\n");
            printf ("%.0f ", matrix_b[i]);
        }

        printf ("\n");*/
        
        __m512 vec_a, vec_b, aux_vec;
        for (int i = 0; i < m_size; ++i) {
            for (int j = 0; j < m_size; ++j) {
                partial_sum = 0;
                sum = 0;
                for (int k = 0; k < n_vectors; ++k) {
                    vec_a = _mm512_load_ps (&matrix_a[(i * 16 * n_vectors) + (k * 16)]);
                    //printf ("\nvec_a  : ");
                    //for (int i = 0; i < 16; i++) printf ("%.0f ", vec_a[i]);
                    vec_b = _mm512_load_ps (&matrix_b[(j * 16 * n_vectors) + (k * 16)]);
                    //printf ("\nvec_b  : ");
                    //for (int i = 0; i < 16; i++) printf ("%.0f ", vec_b[i]);
                    aux_vec = _mm512_mul_ps (vec_a, vec_b);
                    //printf ("\naux_vec  : ");
                    //for (int i = 0; i < 16; i++) printf ("%.0f ", aux_vec[i]);
                    
                    for (int i = 0; i < 16; i++) sum += aux_vec[i];
                    partial_sum += sum;
                    //printf ("a: %p | b: %p | aux: %p\n", &matrix_a[(i * 16 * n_vectors) + (k * 16)], &matrix_b[(j * 16 * n_vectors) + (k * 16)], &aux_vec[k * 16]);
                }
                matrix_c[(i * m_size) + j] = partial_sum;
                //printf ("\nmatrix_c[%d] = %.0f", (i*m_size)+j, matrix_c[(i * m_size) + j]);
            }
        }

        /*printf ("\n\nc: ");
        for (int i = 0; i < m_size*16*n_vectors; i++) {
            if (i % m_size == 0) printf ("\n");
            printf ("%.0f ", matrix_c[i]);
        }*/

        free(matrix_a);
        free(matrix_b);
        free(matrix_c);

    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }
    return 0;
}