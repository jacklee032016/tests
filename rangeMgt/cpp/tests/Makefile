#
# $Author$
#


include $(RULE_DIR)/Rules.mak

CODE_SUBDIRS = \
		.



CFLAGS += \
	$(PJ_CFLAGS) -I../include -Icatch2 \

		
SRC += \
	testMain.cpp \
	testRange.cpp \
	testRangeSetAdd.cpp \
	testRangeSetGet.cpp \
	testRangeSetDelete.cpp \


LOCAL_ALL := \
		testRange \
		demo \


include $(RULE_DIR)/Makefile.post

LDFLAGS := -L$(BIN_DIR)/lib 

ifeq ($(ARCH),arm)
LDFLAGS += 
else
LDFLAGS += 
endif

LDFLAGS += -lRangeCpp

#  -nostartfiles 
#test01:$(OBJS)
#	$(CXX) -s -o $@ $^ $(LDFLAGS) 
#	$(MOVE) $@ $(BIN_DIR)/usr/bin

testRange:$(OBJS)
	$(CXX) -s -o $@ $^ $(LDFLAGS) 
	$(MOVE) $@ $(BIN_DIR)/usr/bin

demo:$(OBJ_DIR)/rangeDemo.o
	$(CXX) -s -o $@ $^ $(LDFLAGS) 
	$(MOVE) $@ $(BIN_DIR)/usr/bin


