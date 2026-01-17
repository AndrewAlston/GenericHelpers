//
// Created by andrew on 1/17/26.
//

#ifndef GENERICHELPER_DECODE_HELPERS_H
#define GENERICHELPER_DECODE_HELPERS_H

#include <asm-generic/types.h>

/** @def proto_msg
 * @brief This structure contains elements used after decoding a protobuf string
 */
struct proto_msg {
    __u64 result; /**<  The 64 bit variable integer decode */
    __u64 msg_code; /**<  The message code derived from the result */
    __u64 msg_type; /**<  The message type derived from the result */
};

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

/** @brief This function is used to decode protobuf messages and message types
 * @details This function reads a maximum of 10 bytes of memory into an array,
 * terminating when one byte does not have its high order bit set.
 * It then reverses the array, and concatenates the byte array into a
 * single 64 bit integer, using only the low order 7 bits of each byte in the
 * array.
 * The result is then shifted right by 3 to produce the message code and
 * AND'd by 7 to get the high order bytes to find the message type
 * NOTE: It is important that this function returns allocated memory, and the
 * result must be free'd by the calling function to avoid memory leaks.
 * @param[in] ptr A pointer to the memory containing the bytes requiring decode
 * @returns An allocated proto_msg structure or NULL if allocation of memory fails
 */
struct proto_msg *mem_to_msg(const __u8 *ptr);

#endif //GENERICHELPER_DECODE_HELPERS_H