include ../inc.mk

CSRC = decode.c

ifdef PORTABLE
  CSRC += converts_portable.c
else
  SSRC = converts.S
endif

ifdef CONSTANT_TIME
  ifdef PORTABLE
    CSRC += secure_decode_portable.c
  else
    SSRC += secure_decode${SUF}.S
  endif
else
  ifdef PORTABLE
    CSRC += decode_portable.c
  else
    SSRC += decode${SUF}.S 
  endif
endif

include ../rules.mk