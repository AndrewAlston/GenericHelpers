//
// Created by andrew on 1/17/26.
//

#ifndef GENERICHELPER_DECODE_HELPERS_H
#define GENERICHELPER_DECODE_HELPERS_H

#include <asm-generic/types.h>

/** @brief dump_buffer takes a pointer and dumps a specific number of bytes
 * @details This function dumps memory in a format that is importable by wireshark or other
 * programs that can import hex dumps
 * @param[in] buffer A pointer to the memory to be dumped
 * @param[in] size The number of bytes to dump
 */
void dump_buffer(void *buffer,__u16 size);

/** @brief This function reverses a 10 byte array
 * @details This function is used primarily for handling variable integer decodes
 * VARINT's are something used heavily in gbp file formats (Google protobuffers)
 * NOTE: This function assumes that the input array is at least 10 bytes long
 * and should the input array be less than 10 bytes this may cause unexpected
 * behavior
 * @param[in] bytes An array of bytes to be reversed.
 */
void reverse_array_10(__u8 bytes[]);
#endif //GENERICHELPER_DECODE_HELPERS_H