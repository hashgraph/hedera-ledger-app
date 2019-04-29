/* This program reads a message from stdin, detects its type and decodes it.
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pb_decode.h>
#include "CreateAccount.pb.h"

/* This function reads manually the first tag from the stream and finds the
 * corresponding message type. It doesn't yet decode the actual message.
 *
 * Returns a pointer to the MsgType_fields array, as an identifier for the
 * message type. Returns null if the tag is of unknown type or an error occurs.
 */
// const pb_field_t* decode_unionmessage_type(pb_istream_t *stream)
// {
//     pb_wire_type_t wire_type;
//     uint32_t tag;
//     bool eof;

//     while (pb_decode_tag(stream, &wire_type, &tag, &eof))
//     {
//         if (wire_type == PB_WT_STRING)
//         {
//             const pb_field_t *field;
//             for (field = UnionMessage_fields; field->tag != 0; field++)
//             {
//                 if (field->tag == tag && (field->type & PB_LTYPE_SUBMESSAGE))
//                 {
//                     /* Found our field. */
//                     return field->ptr;
//                 }
//             }
//         }
        
//         /* Wasn't our field.. */
//         pb_skip_field(stream, wire_type);
//     }
    
//     return NULL;
// }

// bool decode_unionmessage_contents(pb_istream_t *stream, const pb_field_t fields[], void *dest_struct)
// {
//     pb_istream_t substream;
//     bool status;
//     if (!pb_make_string_substream(stream, &substream))
//         return false;
    
//     status = pb_decode(&substream, fields, dest_struct);
//     pb_close_string_substream(stream, &substream);
//     return status;
// }
bool hex_to_bytes(char* hex, uint8_t *buffer) {

    if(hex == NULL) 
       return false;

    size_t hex_length = strlen(hex);
    if((hex_length % 2) != 0) // must be even
       return false;

    size_t buffer_length = hex_length / 2;

    memset(buffer, 0, buffer_length);

    size_t index = 0;
    while (index < hex_length) {
        char c = hex[index];
        int value = 0;
        if(c >= '0' && c <= '9')
          value = (c - '0');
        else if (c >= 'A' && c <= 'F') 
          value = (10 + (c - 'A'));
        else if (c >= 'a' && c <= 'f')
          value = (10 + (c - 'a'));
        else {
          return false;
        }

        buffer[(index/2)] += value << (((index + 1) % 2) * 4);
        index++;
    }
    return true;
}
void uint64_to_bytes(uint64_t number, uint8_t buffer[8]){
    uint8_t *p = (uint8_t *)&number;
    memset(buffer, 0, 8*sizeof(buffer[0]));
    for(int i = 0; i < 8; i++) {
      buffer[i] = p[7-i];
      printf("%02x ", buffer[i]);
    }
    printf("\n");
}


int main()
{
    char *txn="0a120a0c08c3dc8ce60510f8a084c701120218021202180318a08d062202081e28013213437265617465204163636f756e7420546573745a380a221220c9ae8ce9dbff64da0dc61e3389699bd848a355a93cb474b2beea9df84b93fb5d10f0d193a58d1d1a0030e70738e7074a03088827";
                
    size_t buffer_length = strlen(txn) / 2;
    uint8_t buffer[buffer_length];
    bool status = false;
    status = hex_to_bytes(txn, buffer);
    if (!status) {
        printf("Invalid hex\n");
    }
    status = false;
        
    {
        // /* Allocate space for the decoded message. */
        // Transaction message = Transaction_init_default;
        // /* Create a stream that reads from the buffer. */
        // pb_istream_t stream = pb_istream_from_buffer(buffer, buffer_length);
        // /* Now we are ready to decode the message. */
        // status = pb_decode(&stream, Transaction_fields, &message);
        // /* Check for errors... */
        // if (!status)
        // {
        //     PRINTF("Decoding failed: %s\n", PB_GET_ERROR(&stream));
        // }
        
        /* Create a stream that reads from the buffer. */
        //pb_istream_t streamBody = pb_istream_from_buffer(message.bodyData.bodyBytes.bytes, message.bodyData.bodyBytes.size);
        pb_istream_t streamBody = pb_istream_from_buffer(buffer, buffer_length);
        /* Now we are ready to decode the message. */
        TransactionBody messageBody = TransactionBody_init_default;
        status = pb_decode(&streamBody, TransactionBody_fields, &messageBody);
        /* Check for errors... */
        if (!status){
            printf("Decoding body failed: %s\n", PB_GET_ERROR(&streamBody));
        }
        
        /* Print the buffer contained in the message. */
        uint8_t result_buffer[8],result_buffer2[8];
        uint64_to_bytes(messageBody.transactionID.accountID.accountNum, result_buffer);
        printf("messageBody.transactionID.accountID.accountNum: ");
        for (int i=0;i<8;i++) {
          printf("%02X", result_buffer[i]);
        }
        printf("\n");
        uint64_to_bytes(messageBody.data.cryptoCreateAccount.autoRenewPeriod.seconds, result_buffer2);
        printf("messageBody.data.cryptoCreateAccount.autoRenewPeriod.seconds: ");
        for (int i=0;i<8;i++) {
          printf("%02X", result_buffer2[i]);
        }
        printf("\n");
    }
    /* Read the data into buffer */
    // uint8_t buffer[512];
    // size_t count = fread(buffer, 1, sizeof(buffer), stdin);
    // pb_istream_t stream = pb_istream_from_buffer(buffer, count);
    
    // const pb_field_t *type = decode_unionmessage_type(&stream);
    // bool status = false;
    
    // if (type == MsgType1_fields)
    // {
    //     MsgType1 msg = {};
    //     status = decode_unionmessage_contents(&stream, MsgType1_fields, &msg);
    //     printf("Got MsgType1: %d\n", msg.value);
    // }
    // else if (type == MsgType2_fields)
    // {
    //     MsgType2 msg = {};
    //     status = decode_unionmessage_contents(&stream, MsgType2_fields, &msg);
    //     printf("Got MsgType2: %s\n", msg.value ? "true" : "false");
    // }
    // else if (type == MsgType3_fields)
    // {
    //     MsgType3 msg = {};
    //     status = decode_unionmessage_contents(&stream, MsgType3_fields, &msg);
    //     printf("Got MsgType3: %d %d\n", msg.value1, msg.value2);    
    // }
    
    // if (!status)
    // {
    //     printf("Decode failed: %s\n", PB_GET_ERROR(&stream));
    //     return 1;
    // }
    
    return 0;
}



