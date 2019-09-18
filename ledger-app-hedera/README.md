# Hedera Hashgraph Bolos App for Ledger Nano S

This application is the initial version for the Native support for Hedera Hashgraph Crypto
HBAR currency. This Bolos app currently supports signature for a CryptoTransfer transaction.
Post OA the companion app will support other API and features.

Run `make load` to build and load the application onto the device. After
installing and running the application, you can run `demo.py` to test a
signature over USB.

Note that in order to run `demo.py`, you must install the `secp256k1` Python
package:

```
pip install secp256k1
```

See [Ledger's documentation](http://ledger.readthedocs.io) to get started.
