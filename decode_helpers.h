//
// Created by andrew on 1/17/26.
//

#ifndef GENERICHELPER_DECODE_HELPERS_H
#define GENERICHELPER_DECODE_HELPERS_H

#include <asm-generic/types.h>

/** @def bgp_ipv4_prefix
 * @brief This structure contains a prefix (in network byte order)
 * and a CIDR for the prefix
 */
struct bgp_ipv4_prefix {
    __u32 prefix; /**< The network byte order prefix read from a BGP NLRI */
    __u8 cidr; /** The cidr of the prefix */
    __u8 *next; /**< Pointer to the next entry NLRI entry */
};

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

/** @brief Reads a BGP encoded prefix from an NLRI or withdraw message
 * @sa bgp_ipv4_prefix
 * @detail This function decodes compressed ipv4 prefix's as contained
 * in bgp update messages.  The first byte in the message represents the
 * CIDR mask (The number of bits in the prefix).
 * If the CIDR is fully divisible by 8, then CIDR/8 bytes are copied
 * into the prefix element.  If CIDR is NOT fully divisible by 8 then
 * (CIDR/8)+1 bytes are copied into the prefix element.
 * The next pointer element is set using the input pointer + 1 byte
 * for the CIDR and then X bytes for the prefix itself, where X
 * is either CIDR/8 or (CIDR/8)+1
 * NOTE: This function returns a pointer that must be free'd by the caller
 * to avoid memory leaks.  This function only serves for demonstration
 * purposes because this type of processing would normally be done
 * inline in a BGP update function without the need of additional
 * memory allocation.
 * @param[in] ptr A Pointer to the start of the compressed prefix
*/


void *free_mem_to_msg(struct proto_msg *msg);

/** @brief Frees the Pointer returned by a previous successful call to mem_to_msg
 *
 *  NOTE: This function should only be called on a pointer previously returned by mem_to_msg
 *
*/

struct bgp_ipv4_prefix *read_bgp_prefix(__u8 *ptr);

/** @brief this function gets a variable encoded 64 bit integer
 * from a 7 byte encoded string of bytes.
 * @param[in] data A pointer to the bytes we are extracting the
 * variable integer from
 * @param[in] varint A pointer to where the extracted varint will be stored
 * @returns The number of bytes used to encode the extracted varint
 */

void *free_bgp_prefix(struct bgp_ipv4_prefix *prefix);

/** @brief Frees the Pointer returned by a previous successful call to read_bgp_prefix
 *
 *  NOTE: This function should only be called on a pointer previously returned by read_bgp_prefix
 *
*/

__u8 get_var_int(const u_char *data, __u64 *varint);

#endif //GENERICHELPER_DECODE_HELPERS_H
