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

#endif //GENERICHELPER_DECODE_HELPERS_H