//
// Created by andrew on 1/17/26.
//
#include <stdio.h>
#include <stdint.h>
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
            snprintf (ret_print,10,"\n%06x\t",p + 1);
        printf ("%02x%s",*(uint8_t *) (cur_pkt + p),
                ((p + 1) % 16 == 0)?ret_print:" ");
        fflush (stdout);
    }
    printf ("\n\n");
    fflush (stdout);
}
