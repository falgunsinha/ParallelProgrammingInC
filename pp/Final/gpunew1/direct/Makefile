PROJ=direct

CC=gcc

CFLAGS=-std=c99 -Wall -DUNIX -g -DDEBUG -O3

CUDA_INSTALL_PATH =$(CUDA_HOME)

CUDA=${CUDA_INSTALL_PATH}

# 64-bit or 32-bit
PROC_TYPE = $(strip $(shell uname -m | grep 64))
 
LIBS=-lOpenCL
ifeq ($(PROC_TYPE),)
	CFLAGS+=-m32
else
	CFLAGS+=-m64
endif

# look for AMD
ifdef AMDAPPSDKROOT
   INC_DIRS=. $(AMDAPPSDKROOT)/include
	ifeq ($(PROC_TYPE),)
		LIB_DIRS=$(AMDAPPSDKROOT)/lib/x86
	else
		LIB_DIRS=$(AMDAPPSDKROOT)/lib/x86_64
	endif
else

# look for nvidia nvidia
ifdef CUDA
   INC_DIRS=. $(CUDA)/include
	ifeq ($(PROC_TYPE),)
		LIB_DIRS=$(CUDA)/lib32
	else
		LIB_DIRS=$(CUDA)/lib64
	endif
endif

endif


$(PROJ): $(PROJ).c
	$(CC) $(CFLAGS) -o $@ $^ $(INC_DIRS:%=-I%) $(LIB_DIRS:%=-L%) $(LIBS)

.PHONY: clean

clean:
	rm $(PROJ)
