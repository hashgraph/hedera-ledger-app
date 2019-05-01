/* This program reads a message from stdin, detects its type and decodes it.
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pb_decode.h>
#include "CreateAccount.pb.h"

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
bool print_string(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    printf("decoding string\n");
    uint8_t buffer[1024] = {0};
    
    /* We could read block-by-block to avoid the large buffer... */
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    printf("starting to read %ld\n",stream->bytes_left);
    if (!pb_read(stream, buffer, stream->bytes_left)) {
        printf("returning false\n");
        return false;
    }
    /* Print the string, in format comparable with protoc --decode.
     * Format comes from the arg defined in main().
     */
    //printf((char*)*arg, buffer);
    printf("%s\n", buffer);
    *arg=&buffer;
    return true;
}
int main()
{
    //char *txn="120022710a120a0c08c3dc8ce60510f8a084c701120218021202180318a08d062202081e28013213437265617465204163636f756e7420546573745a380a221220c9ae8ce9dbff64da0dc61e3389699bd848a355a93cb474b2beea9df84b93fb5d10f0d193a58d1d1a0030e70738e7074a03088827";
    char *txn="1200223f0a130a0c0888eca1e6051088b6f0d002120318ed071202180318a08d062202081e2801320e557064617465204163636f756e747a0a120318ec07420308aa46";
    size_t buffer_length = strlen(txn) / 2;
    uint8_t buffer[buffer_length];
    bool status = false;
    status = hex_to_bytes(txn, buffer);
    if (!status) {
        printf("Invalid hex\n");
    }
    status = false;
        
    {
        /* Allocate space for the decoded message. */
        Transaction message = Transaction_init_default;
        /* Create a stream that reads from the buffer. */
        pb_istream_t stream = pb_istream_from_buffer(buffer, buffer_length);
        /* Now we are ready to decode the message. */
        status = pb_decode(&stream, Transaction_fields, &message);
        /* Check for errors... */
        if (!status)
        {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
        }
        
        /* Create a stream that reads from the buffer. */
        pb_istream_t streamBody = pb_istream_from_buffer(message.bodyData.bodyBytes.bytes, message.bodyData.bodyBytes.size);
        // pb_istream_t streamBody = pb_istream_from_buffer(buffer, buffer_length);
        /* Now we are ready to decode the message. */
        TransactionBody messageBody = TransactionBody_init_default;
        messageBody.memo.funcs.decode = &print_string;
        messageBody.memo.arg = "stringvalue: \"%s\"\n";
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
        uint64_to_bytes(messageBody.data.cryptoUpdateAccount.autoRenewPeriod.seconds, result_buffer2);
        printf("messageBody.data.cryptoUpdateAccount.autoRenewPeriod.seconds: ");
        for (int i=0;i<8;i++) {
          printf("%02X", result_buffer2[i]);
        }
        printf("\n");
        printf("messageBody.memo: %s\n", (char *)messageBody.memo.arg);
    }
    
    return 0;
}