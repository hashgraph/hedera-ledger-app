# Include the nanopb provided Makefile rules
include ../../extra/nanopb.mk

# Compiler flags to enable all warnings & debug info
CFLAGS = -Wall -Werror -g -O0
CFLAGS += -I$(NANOPB_DIR)

# C source code files that are required
CSRC  = decode.c                   # The main program
CSRC += CreateAccount.pb.c                # The compiled protocol definition
CSRC += $(NANOPB_DIR)/pb_encode.c  # The nanopb encoder
CSRC += $(NANOPB_DIR)/pb_decode.c  # The nanopb decoder
CSRC += $(NANOPB_DIR)/pb_common.c  # The nanopb common parts

# Build rule for the main program
decode: $(CSRC)
	$(CC) $(CFLAGS) -odecode $(CSRC)

# Build rule for the protocol
CreateAccount.pb.c: CreateAccount.proto
	$(PROTOC) $(PROTOC_OPTS) --nanopb_out=. CreateAccount.proto

