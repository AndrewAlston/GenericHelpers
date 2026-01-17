//
// Created by andrew on 1/17/26.
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "decode_helpers.h"

void dump_buffer(void *buffer,__u16 size)
{
    char ret_print[18] = {0};
    void *cur_pkt = buffer;
    printf ("%06x\t",0);
    fflush (stdout);
    for(int p = 0; p < size; p++)
    {
        if((p + 1) % 16 == 0)
	{
            snprintf (ret_print,10,"\n%06x\t",p + 1);
	}
	printf ("%02x%s",*(uint8_t *) (cur_pkt + p),
                ((p + 1) % 16 == 0)?ret_print:" ");
        fflush (stdout);
    }
    printf ("\n\n");
    fflush (stdout);
}

void reverse_array_10(__u8 bytes[])
{
    __u8 size = 10;
    for(int i = 0; i < size/2; i++) {
        __u8 temp = bytes[i];
        bytes[i] = bytes[size-1-i];
        bytes[size-1-i] = temp;
    }
}

struct proto_msg *mem_to_msg(const __u8 *ptr)
{
    int count = 0;
    __u8 bytes[10];
    struct proto_msg *result = calloc(1, sizeof(struct proto_msg));
    if (!result) {
        return NULL;
    }

    for (int i = 9; i > 0; i++) {
        if ((ptr[count] & 128) == 128) {
            bytes[i] = ptr[count++];
            continue;
        }
        bytes[i] = ptr[count];
        break;
    }

    reverse_array_10(bytes);
    
    for (int i = 0; i < 10; i++) {
        __u64 masked = bytes[i]&0x7F;
        result->result |= (masked << (i*7));
    }
    result->msg_code = result->result>>3;
    result->msg_type = result->result&7;
    return result;
}

void *free_mem_to_msg(struct proto_msg *msg)
{
	if(msg == NULL)
	{
		return;
	}

	free(msg);
}

struct bgp_ipv4_prefix *read_bgp_prefix(__u8 *ptr) {
    if (!ptr || ptr[0] > 32){
        return NULL;
    }

    struct bgp_ipv4_prefix *pfx = calloc(1, sizeof(struct bgp_ipv4_prefix));
    if (!pfx){
        return NULL;
    }

    pfx->cidr = ptr[0];
    if (pfx->cidr%8) {
        memcpy(&pfx->prefix, ptr+1, (pfx->cidr/8)+1);
        pfx->next = ptr+(pfx->cidr/8)+2;
    } else {
        memcpy(&pfx->prefix, ptr+1, pfx->cidr/8);
        pfx->next = ptr+(pfx->cidr/8)+1;
    }
    return pfx;
}

void *free_bgp_prefix(struct bgp_ipv4_prefix *prefix)
{
	if(prefix == NULL)
	{
		return;
	}

	free(prefix);
}


__u8 get_var_int(const u_char *data, __u64 *varint)
{
    if (!data || !varint) {
        return 0;
    }
    __u8 bytes[10];
    __u8 data_count = 0;
    __u64 byte_count = 0;
    __u8 length = 9;

    /**< Zero out the varint before we start */
    *varint = 0;
    /**< If the first byte does not have the high order bit set
     * take that as a standalone byte, else transverse the memory
     * pointed to by data until either we have 10 bytes or until
     * the high order bit is not set */
    if ((data[0] & 128) == 128) {
        bytes[length] = data[data_count];
        byte_count = 1;
    } else {
        while ((data[data_count] & 128) == 128) {
            bytes[length--] = data[data_count++];
            byte_count++;
            if (byte_count == 9)
                break;
        }
        bytes[length] = data[data_count];
    }
    reverse_array_10(bytes);
    for (int i = 0; i < 10; i++) {
        __u64 masked = bytes[i]&0x7F;
        *varint |= (masked << (i&7));
    }
    return byte_count;
}
