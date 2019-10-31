# Makefile 
# 
#


include $(RULE_DIR)/Rules.mak


###################################################################
# define directory for object files
###################################################################


CODE_SUBDIRS = \
	. \


# --std=c++14
CFLAGS += \
	-I../include \
	


###################################################################
# define source files
###################################################################




SRC += \
	rangeTree.c \
	rangeSet.c \


TARGET_LIB:=libRange

TARGET_LIB_SHARED:=$(TARGET_LIB).so
TARGET_LIB_STATIC:=$(TARGET_LIB).a
		 
LOCAL_ALL:=

include $(RULE_DIR)/Makefile.post
