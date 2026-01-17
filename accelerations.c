//
// Created by andrew on 1/17/26.
//

#include <asm-generic/types.h>
#include "accelerations.h"

#ifdef AVX512
#include <immintrin.h>

int search_array_32(void *haystack, __u64 offset, size_t struct_size, int array_size, __u32 needle)
{
    __u64 offset_array[8] = {
        offset, (__u64)haystack+offset, (__u64)(haystack+offset)*2, (__u64)(haystack+offset)*3,
        (__u64)(haystack+offset)*4, (__u64)(haystack+offset*5), (__u64)(haystack+offset)*6,
        (__u64)(haystack+offset)*7
    };
    void *start = haystack+offset;
    __m512i offset_vec = _mm512_loadu_epi64(offset_array);
    __m256i needle_vec = _mm256_set1_epi32(needle);
    int loop_increment = 8*struct_size;
    for (int i = 0; i<= array_size-8; i+=8) {
        __m256i gather_vec = _mm512_i64gather_epi32(offset_vec, start, 1);
        __mmask8 result = __mm256_comeq_epi32_mask(gather_vec, needle_vec);
        if ((mask_result = _cvtmask8_u32(result)) != 0)
            return (int)i+__builtin_ctz(mask_result);
        start += loop_increment;
    }
    return -1;
}
#endif