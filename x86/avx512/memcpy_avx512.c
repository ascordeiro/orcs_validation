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
    for (int i = 0; i < v_size; i++) data_a[i] = 5;
    int* data_b = (int*) aligned_alloc (32, v_size*sizeof (int));
    
    __m512i vec_a, vec_b;
    for (int i = 0; i < v_size; i += 16) {
        vec_a = _mm512_load_si512 ((__m512i *) &data_a[i]);
        _mm512_stream_si512 ((__m512i *) &data_b[i], vec_a);
    }

    printf ("%d\n", data_b[v_size-1]);
    return 0;
}
