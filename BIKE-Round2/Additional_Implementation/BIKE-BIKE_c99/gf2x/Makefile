include ../inc.mk

ifdef USE_OPENSSL
  CSRC = openssl_utils.c
else
  CPPSRC = ntl.cpp
  CSRC = gf2x_mul.c
  SSRC = gf_add.S  gf_mul.S  red.S
endif

include ../rules.mk


