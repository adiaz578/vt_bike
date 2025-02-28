export ROOT = $(realpath .)
export OBJ_DIR = ${ROOT}/obj/

include inc.mk

BIN_DIR = ./bin/
TARGET := $(BIN_DIR)/main

SUB_DIRS := prf 
SUB_DIRS += decode 
SUB_DIRS += hash 
SUB_DIRS += gf2x
SUB_DIRS += common

CSRC = kem.c

OBJS = $(OBJ_DIR)/*.o
ifdef USE_NIST_RAND
    CSRC += FromNIST/rng.c FromNIST/PQCgenKAT_kem.c
    OBJS += $(OBJ_DIR)/FromNIST/*.o
else
    SUB_DIRS += tests
endif

ifdef USE_OPENSSL
    EXTERNAL_LIBS = -lcrypto -ldl -lpthread
else
    EXTERNAL_LIBS = -lntl -lgf2x -lgmp -lpthread
endif

.PHONY: $(SUB_DIRS)

include rules.mk

all: $(BIN_DIR) $(OBJ_DIR) $(SUB_DIRS)
	$(CPP)  $(OBJS) $(CPPFLAGS) $(EXTERNAL_LIBS) -o $(TARGET)

$(SUB_DIRS):
	make -C $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/FromNIST

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)