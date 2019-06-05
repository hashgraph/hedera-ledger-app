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
from ledgerblue.commException import CommException
import argparse
import struct

def parse_bip32_path(path):
	if len(path) == 0:
		return ""
	result = ""
	elements = path.split('/')
	for pathElement in elements:
		element = pathElement.split('\'')
		if len(element) == 1:
			result = result + struct.pack(">I", int(element[0]))			
		else:
			result = result + struct.pack(">I", 0x80000000 | int(element[0]))
	return result

parser = argparse.ArgumentParser()
parser.add_argument('--path', help="BIP 32 path to sign with")
parser.add_argument('--tx', help="TX to sign, hex encoded")
args = parser.parse_args()

if args.path == None:
	args.path="44'/3030'/0'/0'/0'"
#if args.tx == None:
#	raise Exception("Missing TX")

#args.tx = args.tx.decode('hex')

donglePath = parse_bip32_path(args.path)

pathLength = len(donglePath) + 1
p1 = "80"
p2 = "80"
data1 = ("0a130a0c08e9b2ace60510e088a88103120318ce101202180318a08d062202081e320d54657374205472616e7366657272140a120a070a0318cc1010010a070a0318cd101002").decode('hex')
#apdu = ("e002" + p1 + p2).decode('hex') + chr(pathLength) + chr(len(donglePath) / 4) + donglePath
apdu = ("e004" + p1 + p2).decode('hex') + chr(pathLength + len(data1)) + chr(len(donglePath) / 4) + donglePath + data1

dongle = getDongle(True)
result = dongle.exchange(bytes(apdu))
#0a130a0c08e9b2ace60510e088a88103120318ce101202180318a08d062202081e320d54657374205472616e7366657272140a120a070a0318cc1010010a070a0318cd101002
#if data2 != None:
#    apdu = "e0048140".decode('hex') + chr(len(data2)) + data2
#    result = dongle.exchange(bytes(apdu))

#result = dongle.exchange(bytes("e0040040a0058000002c80000090800000008000000000000000000b207900039f97d5a32fa1a1a0e12920f5ed21fde918a11ebf14cd42b70b4cc3633b027fcf175fa2a273f5a34d65710e67fc2b4bfe50d0d8140a0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e9a435000000008096980000000000".decode('hex')))
# result = dongle.exchange(bytes("e002004096058000002c80000090800000008000000000000000".decode('hex')))


#print "publicKey " + str(result).encode('hex')

