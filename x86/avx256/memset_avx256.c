#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>

int main(int argc, char const *argv[]) {
    int v_size;
    int size = atoi(argv[1]);
    if (size != 0 && (size & (size - 1)) == 0) v_size = (1024 * 1024 * size) / sizeof(int);
    else return 0;

    int* data_a = (int*) aligned_alloc (32, v_size*sizeof (int));

    __m256i vec_a;
    for (int i = 0; i < v_size; i += 8) {
        vec_a = _mm256_load_si256 ((__m256i *) &data_a[i]);
        vec_a = _mm256_set1_epi32(1);
        _mm256_store_si256 ((__m256i *) &data_a[i], vec_a);
    }

    return 0;
}
