# rule for all modules

############## Common for all modules
# support PC environments
# ARCH=


# released or debug version, different on debug and log info��2007.03.15
# must be 'release' or 'debug'
# EDITION=debug
EDITION=release

ifeq ($(EDITION),release)
	CFLAGS += -D__EXT_RELEASE__
	CXXFLAGS += -D__EXT_RELEASE__
else	
	CFLAGS += -DDEBUG=1
	CXXFLAGS += -DDEBUG=1
endif 

ifeq ($(ARCH),arm)
#	C_FLAGS += -D__ARM_CMN__=1 -DARCH_ARM=1  -DARCH_X86=0 -DARCH_X86_32=0 
	CROSS_COMPILER=arm-none-eabi-
	LDFLAGS+=  
	flag=
	C_FLAGS +=-DARM -DARCH_ARM=1 
	
else
	ARCH=X86
	C_FLAGS +=-D$(ARCH) -DARCH_X86=1 -DARCH_ARM=0 
	EXTENSION=
endif


ifeq ($(ARCH),X86)
else
	ARCH=arm
endif


CC	= $(CROSS_COMPILER)gcc
CXX 	= $(CROSS_COMPILER)g++
STRIP	= $(CROSS_COMPILER)strip
LD	= $(CROSS_COMPILER)ld
RANLIB 	= $(CROSS_COMPILER)ranlib
STRIP 	= $(CROSS_COMPILER)strip
AR 	= $(CROSS_COMPILER)ar
OBJCOPY 	= $(CROSS_COMPILER)objcopy

ASM = yasm

RM	= rm -r -f
MKDIR	= mkdir -p
MODE	= 700
OWNER	= root
CHOWN	= chown
CHMOD	= chmod
COPY	= cp
MOVE	= mv

LN		= ln -sf


# configuration options for manage this project
#BUILDTIME := $(shell TZ=UTC date -u "+%Y_%m_%d-%H_%M")
BUILDTIME := $(shell TZ=CN date -u "+%Y_%m_%d")
GCC_VERSION := $(shell $(CC) -dumpversion )

RELEASES_NAME=$(NAME)_$(GCC_VERSION)_$(ARCH)_$(EDITION)_$(BUILDTIME).tar.gz  


export BUILDTIME
export RELEASES_NAME


BIN_DIR=$(ROOT_DIR)/Linux.bin.$(ARCH)
OBJ_DIR=Linux.obj.$(ARCH)


############## definitions for different modules


EXE=rtosLwip$(BOARD_NAME).bin


#  -I$(LWIP_HOME)/project \



CFLAGS += -DROOT_DIR='"$(ROOT_DIR)"' -I$(ROOT_DIR) -I$(ROOT_DIR)/include 


###################################################################
# define directories for header file and build flags
###################################################################


RTOS_FLAGS+=

CWARNS += -W
CWARNS += -Wall
#CWARNS += -Werror
CWARNS += -Wextra
# -Wno-format
CWARNS += -Wformat=0 
CWARNS += -Wmissing-braces -Wmissing-declarations -Wmissing-format-attribute 
# CWARNS += -Wmissing-prototypes
CWARNS += -Wno-cast-align
CWARNS += -Wparentheses
CWARNS += -Wshadow
CWARNS += -Wno-sign-compare
CWARNS += -Wswitch
CWARNS += -Wuninitialized
CWARNS += -Wunknown-pragmas
CWARNS += -Wunused-function
CWARNS += -Wunused-label
# CWARNS += -Wunused-parameter
CWARNS += -Wunused-value
#CWARNS += -Wunused-variable

CWARNS +=  -fno-strict-aliasing

#CWARNS += -Wno-unused-function

CW_WARNS += -Wstrict-prototypes -Wpointer-arith -Wchar-subscripts -Wcast-align 

# -std=gnu99 

CFLAGS += \
	-O1 -g3 \
	 \
	-Wcomment -Wmain -Wparentheses -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs \
	-Wunused -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef -Wwrite-strings -Wsign-compare -Waggregate-return \
	-Wno-deprecated-declarations -Wredundant-decls \
	-Wunreachable-code 
	
# -Wimplicit-int -Wbad-function-cast 

# only for 32bit system
# CFLAGS += -m32

#CFLAGS += -g -DUSE_STDIO=1 -D__GCC_POSIX__=1
CFLAGS += -g -UUSE_STDIO -D__GCC_POSIX__=1
ifneq ($(shell uname), Darwin)
CFLAGS += -pthread
endif

# MAX_NUMBER_OF_TASKS = max pthreads used in the POSIX port. 
# Default value is 64 (_POSIX_THREAD_THREADS_MAX), the minimum number required by POSIX.
CFLAGS += -DMAX_NUMBER_OF_TASKS=300

CFLAGS += $(INCLUDES) $(CWARNS) -O2 $(PROJECT_CFLAGS)


# can't use this options, pbuf in Lwip and gmac of atmel must be alligned differently, such as 8 bytes border or others
# CPACK_FLAGS =	-fpack-struct
# -fpack-struct, add 05.07,2018,JL

# -Wlong-long: disable ISO C90 not support 'long long' warns in RTK SDK
# -Wpacked : disable warns for 'packed' in LwIP protocols
# -Wnested-externs : nested extern declaration

# for FreeRTOS, declarations of sysclk_get_cpu_hz() 
#CFLAGS += -Werror=implicit-function-declaration	

