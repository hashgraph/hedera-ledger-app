/* This program reads a message from stdin, detects its type and decodes it.
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pb_encode.h>
#include <pb_decode.h>
#include "CreateAccount.pb.h"
#include "util.h"
#include <inttypes.h>

#define MEMO_MAX_SIZE 128
#define ACCOUNT_AMOUNT_MAX_SIZE 2

typedef struct {
    uint8_t buffer[MEMO_MAX_SIZE];
    size_t buffer_length;
} memo_t;
typedef struct {
    AccountAmount amounts[MEMO_MAX_SIZE];
    size_t amounts_length;
} account_amounts_t;

void memo_add_array(memo_t *memo, uint8_t source[], size_t length)
{
    size_t i=0;
    for(i=0; i<length; i++) {
        if(memo->buffer_length < MEMO_MAX_SIZE) {
            memo->buffer[memo->buffer_length] = source[i];
            memo->buffer_length++;
        }
    }
}

void add_account_amount(account_amounts_t *account_amounts, AccountAmount account_amount)
{
    if(account_amounts->amounts_length < ACCOUNT_AMOUNT_MAX_SIZE) {
        account_amounts->amounts[account_amounts->amounts_length] = account_amount;
        account_amounts->amounts_length++;
    }
}

bool decode_memo(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    uint8_t buffer[MEMO_MAX_SIZE] = {0};
    memo_t *memo = (memo_t *)(*arg);
    size_t length;
    /* Read bytes_left or 128 bytes whichever is less */
    if (stream->bytes_left > MEMO_MAX_SIZE){
        //more than 128 bytes so read first 128 bytes
        length = MEMO_MAX_SIZE; 
        if (!pb_read(stream, buffer, length))
            return false;
        //read to the end of field
        pb_read(stream, NULL, stream->bytes_left - length);
    } else {
        //read bytes_left  bytes
        length = stream->bytes_left;
        if (!pb_read(stream, buffer, length))
            return false;
    }
    
    //add buffer to memo
    memo_add_array(memo, buffer, length);
    
    return true;
}

bool decode_account_amounts(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    // account_amounts_t *account_amounts = (account_amounts_t *)(*arg);
    //read AccountAmount from the stream
    AccountAmount account_amount = AccountAmount_init_zero;
    if (!pb_decode(stream, AccountAmount_fields, &account_amount))
        return false;
    
    printf("amount = %ld\n",account_amount.amount);
    //add account_amount to account_amounts
    // add_account_amount(account_amounts, account_amount);

    return true;
}
int main()
{
    // //char *txn="120022710a120a0c08c3dc8ce60510f8a084c701120218021202180318a08d062202081e28013213437265617465204163636f756e7420546573745a380a221220c9ae8ce9dbff64da0dc61e3389699bd848a355a93cb474b2beea9df84b93fb5d10f0d193a58d1d1a0030e70738e7074a03088827";
    // char *txn="120022460a130a0c08e9b2ace60510e088a88103120318ce101202180318a08d062202081e320d54657374205472616e7366657272140a120a070a0318cc1010010a070a0318cd101002";
    // size_t buffer_length = strlen(txn) / 2;
    // uint8_t buffer[buffer_length];
    bool status = false;
    // status = hex_to_bytes(txn, buffer);
    // if (!status) {
    //     printf("Invalid hex\n");
    // }
    // status = false;
    
    uint8_t buf[1024];
    {
        TransactionBody txnBody = TransactionBody_init_zero;
        txnBody.which_data = 11;
        txnBody.data.cryptoCreateAccount.initialBalance = 0xffffffffffffffff;
        printf("txnBody.data.cryptoCreateAccount.initialBalance = %lu\n",txnBody.data.cryptoCreateAccount.initialBalance);
        status = hex_to_bytes("1111111111111111111111111111111111111111111111111111111111111111", txnBody.data.cryptoCreateAccount.key.key.ed25519.bytes);
        txnBody.data.cryptoCreateAccount.key.key.ed25519.size = 32;
        if (!status) {
            printf("Invalid hex\n");
        }
        status = false;
       
        // txnBody.data.cryptoCreateAccount.key.key.ed25519 = 0x1111111111111111111111111111111111111111111111111111111111111111;
        strcpy(txnBody.memo, "Create Account");
        // txnBody.memo = "Create Account";
        txnBody.transactionFee = 100000;
        txnBody.nodeAccountID.accountNum = 3;
        txnBody.transactionID.accountID.accountNum = 1002;
        txnBody.transactionID.transactionValidStart.seconds = 1557414024;
        pb_ostream_t stream1 = pb_ostream_from_buffer(buf, sizeof(buf));
        status = pb_encode(&stream1, TransactionBody_fields, &txnBody);
        size_t message_length = stream1.bytes_written;
        /* Then just check for any errors.. */
        if (!status)
        {
            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream1));
            return 1;
        }
        printf("message_length: %lu\n",message_length);
        printf("bodyBytes: ");
        int i=0;
        for(i=0;i<message_length;i++){
            printf("%02X",buf[i]);
        }
        printf("\n");

        printf("bodyBytes in char: ");
        for(i=0;i<message_length;i++){
            printf("%c",buf[i]);
        }
        printf("\n");
    }
    // status = false;
    // {
    //     /* Allocate space for the decoded message. */
    //     Transaction message = Transaction_init_default;
    //     /* Create a stream that reads from the buffer. */
    //     pb_istream_t stream = pb_istream_from_buffer(buffer, buffer_length);
    //     /* Now we are ready to decode the message. */
    //     status = pb_decode(&stream, Transaction_fields, &message);
    //     /* Check for errors... */
    //     if (!status)
    //     {
    //         printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
    //     }
        
    //     /* Create a stream that reads from the buffer. */
    //     pb_istream_t streamBody = pb_istream_from_buffer(message.bodyData.bodyBytes.bytes, message.bodyData.bodyBytes.size);
    //     // pb_istream_t streamBody = pb_istream_from_buffer(buffer, buffer_length);
    //     /* Now we are ready to decode the message. */
    //     TransactionBody messageBody = TransactionBody_init_default;
        
    //     //initialize empty lists to store objects returned from callbacks
    //     // memo_t memo_decoded = {{0}, 0};
    //     //account_amounts_t account_amounts = {{{{0, 0, 0}, 0},{{0, 0, 0}, 0}} ,0};
    //     //account_amounts_t account_amounts = {{0}, 0};

    //     // account_amounts.amounts_length=0;
    //     // int i=0;
    //     // for(i=0;i<ACCOUNT_AMOUNT_MAX_SIZE;i++) {
    //     //     account_amounts.amounts[i].amount=0;
    //     //     // account_amounts.amounts[i].accountID.accountNum=0;
    //     //     // account_amounts.amounts[i].accountID.realmNum=0;
    //     //     // account_amounts.amounts[i].accountID.shardNum=0;
    //     //     account_amounts.amounts[i] = {{0, 0, 0}, 0};
    //     // }

    //     //messageBody.memo.arg = &memo_decoded;
    //     //messageBody.memo.funcs.decode = &decode_memo;
    //     //messageBody.data.cryptoTransfer.transfers.accountAmounts.arg = &account_amounts;
    //     //messageBody.data.cryptoTransfer.transfers.accountAmounts.funcs.decode = &decode_account_amounts;

    //     status = pb_decode(&streamBody, TransactionBody_fields, &messageBody);
    //     // status = pb_decode(&streamBody, TransactionBody_fields, &messageBody);
    //     /* Check for errors... */
    //     if (!status){
    //         printf("Decoding body failed: %s\n", PB_GET_ERROR(&streamBody));
    //     }
    //     // check for trnsaction type of data field
    //     if (messageBody.which_data == TransactionBody_cryptoCreateAccount_tag) {
    //         // create account transaction
    //         char result_hex[17];
    //         printf("Create account transaction\n");
    //         uint64_to_hex_proper_endian(messageBody.transactionID.accountID.accountNum, result_hex);
    //         printf("messageBody.transactionID.accountID.accountNum: %s \n", result_hex);
    //         uint64_to_hex_proper_endian(messageBody.data.cryptoCreateAccount.initialBalance, result_hex);
    //         printf("messageBody.data.cryptoCreateAccount.initialBalance: %s \n", result_hex);
    //         printf("messageBody.memo: %s\n", messageBody.memo);
    //     } else if (messageBody.which_data == TransactionBody_cryptoUpdateAccount_tag) {
    //         // update account transaction
    //         char result_hex[17];
    //         printf("Update account transaction\n");
    //         uint64_to_hex_proper_endian(messageBody.transactionID.accountID.accountNum, result_hex);
    //         printf("messageBody.transactionID.accountID.accountNum: %s \n", result_hex);
    //         uint64_to_hex_proper_endian(messageBody.data.cryptoUpdateAccount.autoRenewPeriod.seconds, result_hex);
    //         printf("messageBody.data.cryptoUpdateAccount.autoRenewPeriod.seconds: %s \n", result_hex);
    //         printf("messageBody.memo: %s\n", messageBody.memo);
    //     } else if (messageBody.which_data == TransactionBody_cryptoTransfer_tag) {
    //         // crypto transfer transaction
    //         char result_hex[17];
    //         printf("Transfer transaction\n");
    //         uint64_to_hex_proper_endian(messageBody.transactionID.accountID.accountNum, result_hex);
    //         printf("messageBody.transactionID.accountID.accountNum: %s \n", result_hex);
    //         // uint64_to_hex_proper_endian(messageBody.data.cryptoUpdateAccount.autoRenewPeriod.seconds, result_hex);
    //         // printf("messageBody.data.cryptoUpdateAccount.autoRenewPeriod.seconds: %s \n", result_hex);
    //         printf("messageBody.memo: %s\n", messageBody.memo);
    //         printf("messageBody.data.cryptoTransfer.transfers.accountAmounts[0].amount: %ld\n", messageBody.data.cryptoTransfer.transfers.accountAmounts[0].amount);
    //         printf("messageBody.data.cryptoTransfer.transfers.accountAmounts[1].amount: %ld\n", messageBody.data.cryptoTransfer.transfers.accountAmounts[1].amount);
    //     } else {
    //         //TODO: throw unsupported transaction type error
    //     }                    
    // }
    
    return 0;
}