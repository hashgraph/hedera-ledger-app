#!/usr/bin/env python
#*******************************************************************************
#*   Ledger Blue
#*   (c) 2016 Ledger
#*
#*  Licensed under the Apache License, Version 2.0 (the "License");
#*  you may not use this file except in compliance with the License.
#*  You may obtain a copy of the License at
#*
#*      http://www.apache.org/licenses/LICENSE-2.0
#*
#*  Unless required by applicable law or agreed to in writing, software
#*  distributed under the License is distributed on an "AS IS" BASIS,
#*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#*  See the License for the specific language governing permissions and
#*  limitations under the License.
#********************************************************************************
from ledgerblue.comm import getDongle
dongle = getDongle(True)
#### Create account transaction
#chunk = "120022710a120a0c08c3dc8ce60510f8a084c701120218021202180318a08d062202081e28013213437265617465204163636f756e7420546573745a380a221220c9ae8ce9dbff64da0dc61e3389699bd848a355a93cb474b2beea9df84b93fb5d10f0d193a58d1d1a0030e70738e7074a03088827"
#### Update account transaction
#chunk = "1200223f0a130a0c0888eca1e6051088b6f0d002120318ed071202180318a08d062202081e2801320e557064617465204163636f756e747a0a120318ec07420308aa46"
#### Transfer transaction
chunk = "120022460a130a0c08e9b2ace60510e088a88103120318ce101202180318a08d062202081e320d54657374205472616e7366657272140a120a070a0318cc1010010a070a0318cd101002"
p1 = 0x80
apdu = bytes("8002".decode('hex')) + chr(p1) + chr(0x00) + chr(len(chunk)/2) + bytes(chunk.decode('hex'))
dongle.exchange(apdu)

