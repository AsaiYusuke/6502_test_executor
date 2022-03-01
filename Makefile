SRC_DIR			:=	src
INC_DIR			:=	include
BUILD_DIR		:=	build
THIRD_PARTY_DIR	:=	third_party

SRC_DIRS		:=	$(shell find $(SRC_DIR) -type d)
BUILD_DIRS		:=	$(SRC_DIRS:$(SRC_DIR)%=$(BUILD_DIR)%)

TARGET			:=	6502_tester
SOURCES			:=	$(shell find $(SRC_DIR) -type f -name '*.cpp')
HEADERS			:=	$(shell find $(SRC_DIR) -type f -name '*.h')
OBJECTS         :=	$(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

ARGS_DIR		:=	$(THIRD_PARTY_DIR)/args
ARGS_HEADER		:=	$(ARGS_DIR)/args.hxx

JSON_DIR		:=	$(THIRD_PARTY_DIR)/json/single_include
JSON_HEADER		:=	$(JSON_DIR)/nlohmann/json.hpp

MOS6502_DIR		:=	$(THIRD_PARTY_DIR)/mos6502
MOS6502_HEADER	:=	$(MOS6502_DIR)/mos6502.h
MOS6502_CXX		:=	$(MOS6502_DIR)/mos6502.cpp
MOS6502_OBJ		:=	$(MOS6502_DIR)/mos6502.o

CFLAGS			:=	--std=c++17 -I $(INC_DIR) -I $(ARGS_DIR) -I $(JSON_DIR) -I $(MOS6502_DIR) -g

.PHONY : all clean test

all: $(BUILD_DIRS) $(TARGET) test

$(BUILD_DIRS) : % :
	mkdir -p $@

$(TARGET): $(OBJECTS) $(MOS6502_OBJ)
	$(CXX) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(MOS6502_OBJ)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(MOS6502_DIR)/%.o : $(MOS6502_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

test :
	make -C example

clean :
	rm -rf $(BUILD_DIR)
	-rm $(MOS6502_OBJ)
	-rm $(TARGET)

$(foreach SOURCE,$(SOURCES),$(eval $(subst \,,$(shell $(CXX) $(CFLAGS) -MM $(SOURCE) | sed -e 's/^\([^ ]*\)\.o:/$(BUILD_DIR)\/\1.o:/'))))
