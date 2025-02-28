include ../inc.mk

CSRC = parallel_hash.c
ifndef USE_OPENSSL
  CSRC += sha-mb.c sha.c
  SSRC  = sha384_update_mb.S
endif

include ../rules.mk

