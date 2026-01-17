//
// Created by andrew on 1/17/26.
//

#ifndef GENERICHELPER_ACCELERATIONS_H
#define GENERICHELPER_ACCELERATIONS_H

#include <asm-generic/types.h>

#ifdef AVX512
/** @brief Search an array of structures for a particular 32 bit integer
 * @details This function takes a pointer to an array of structures and an offset from the base struct
 * to the 32 element in the structure, and then searches the array of structures for the first element
 * of the array where *(struct_ptr+offset) = needle.
 *
 * NOTE: For this function to work properly the number of array elements has to be divisible by 8
 *
 * By way of example if we have the following
 *
 * struct x {
 *   __u32 a;
 *   __u32 b;
 *   __u32 c;
 * }
 * struct x array[24];
 * If we wanted to search for the first entry where x.b = 10, we would pass the function
 * with the following arguments:
 * array - which would be the pointer to the start of the array,
 * 4 (element b is 4 bytes into the structure),
 * sizeof(struct x) - the size of structure for loop increments,
 * 24 - The number of elements in the array
 * 10 - the needle we are searching for
 * @param[in] haystack - Pointer to the start of the array
 * @param[in] offset - An offset of the struct to the 32bit element we are searching
 * @param[in] struct_size - The size of each struct in the struct array
 * @param[in] array_size - The number of elements in the array
 * @param[in] needle - The needle we are searching for
 * @returns The array offset for the first matching instance or -1 if not found
 */
int search_array_32(void *haystack, __u64 offset, size_t struct_size, size_t array_size, __u32 needle);
#endif

#endif //GENERICHELPER_ACCELERATIONS_H