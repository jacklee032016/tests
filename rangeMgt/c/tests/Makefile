#
# $Author$
#


include $(RULE_DIR)/Rules.mak

CODE_SUBDIRS = \
		.



CFLAGS += \
	$(PJ_CFLAGS) -I../include \

		
SRC += \
	testRangeTree.c \


LOCAL_ALL := \
		testTree \


include $(RULE_DIR)/Makefile.post

LDFLAGS := -L$(BIN_DIR)/lib 

ifeq ($(ARCH),arm)
LDFLAGS += 
else
LDFLAGS += 
endif

LDFLAGS += -lRange


testTree:$(OBJS)
	$(CXX) -s -o $@ $^ $(LDFLAGS) 
	$(MOVE) $@ $(BIN_DIR)/usr/bin
