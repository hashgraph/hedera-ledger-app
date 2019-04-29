/* This program takes a command line argument and encodes a message in
 * one of MsgType1, MsgType2 or MsgType3.
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pb_encode.h>
#include "CreateAccount.pb.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s (1|2|3)\n", argv[0]);
        return 1;
    }
    
    /*uint8_t buffer[512];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    size_t message_length;
    bool status = false;*/
    return 0;
}


