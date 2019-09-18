# ledger-app-hedera
This is a ledger Nano S wallet implementation for Hedera Hashgraph Network.
This application is designed to support the BIP39 - ED25519 keys only .

Currently this application supports the following functions
1) View Public Key & Validate using companion app
2) Perform Crypto transfer that reads the proto message and shows the following 
  - From Account ID
  - TO Account ID
  - Amount being sent in TinyBars
  - Confirmation
3) Fetch records from the account by paying using the transfer

Future Support & Feature requests 
- Account Creation
- GetAccount Info
- Other Hedera Hashgraph API 

# dependencies
nanopb implementation for protobuf support.

# supported sdk version
This app requires minimum version of 1.5 for the SDK.
 
