SRC_DIR				:=	src
INC_DIR				:=	include
BUILD_DIR			:=	build
THIRD_PARTY_DIR		:=	third_party

SRC_DIRS			:=	$(shell find $(SRC_DIR) -type d)
BUILD_DIRS			:=	$(SRC_DIRS:$(SRC_DIR)%=$(BUILD_DIR)%)

TARGET				:=	6502_tester
SOURCES				:=	$(shell find $(SRC_DIR) -type f -name '*.cpp')
HEADERS				:=	$(shell find $(SRC_DIR) -type f -name '*.h')
OBJECTS				:=	$(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

ARGS_DIR			:=	$(THIRD_PARTY_DIR)/args
ARGS_HEADER			:=	$(ARGS_DIR)/args.hxx

JSON_DIR			:=	$(THIRD_PARTY_DIR)/json/single_include
JSON_HEADER			:=	$(JSON_DIR)/nlohmann/json.hpp

MOS6502_DIR			:=	$(THIRD_PARTY_DIR)/mos6502
MOS6502_PATCH_DIR	:=	$(THIRD_PARTY_DIR)/mos6502_patch
MOS6502_PATCH		:=	$(MOS6502_PATCH_DIR)/mos6502.patch
MOS6502_BUILD_DIR	:=	$(BUILD_DIR)/mos6502
MOS6502_CXX			:=	$(MOS6502_BUILD_DIR)/mos6502.cpp
MOS6502_PATCHED		:=	$(MOS6502_BUILD_DIR)/.patched
MOS6502_OBJ			:=	$(BUILD_DIR)/mos6502.o

CFLAGS			:=	--std=c++20 -I $(INC_DIR) -I $(ARGS_DIR) -I $(JSON_DIR) -I $(MOS6502_BUILD_DIR) -I $(MOS6502_DIR) -g

.PHONY : all exec clean test compile_commands

all: exec test

exec: compile_commands $(BUILD_DIRS) $(TARGET)

compile_commands:
	python3 tools/generate_compile_commands.py "$(CURDIR)" "$(BUILD_DIR)" "$(CXX)" "$(CFLAGS)" "$(SOURCES)" "$(MOS6502_CXX)" "$(MOS6502_OBJ)"

$(BUILD_DIRS) : % :
	mkdir -p $@

$(TARGET) : $(MOS6502_OBJ) $(OBJECTS)
	$(CXX) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(MOS6502_OBJ)

$(MOS6502_PATCHED) : $(MOS6502_DIR)/mos6502.cpp $(MOS6502_DIR)/mos6502.h $(MOS6502_PATCH)
	mkdir -p $(MOS6502_BUILD_DIR)
	cp $(MOS6502_DIR)/mos6502.cpp $(MOS6502_DIR)/mos6502.h $(MOS6502_BUILD_DIR)/
	patch --directory=$(MOS6502_BUILD_DIR) --input=$(abspath $(MOS6502_PATCH))
	touch $@

$(OBJECTS) : | $(MOS6502_PATCHED)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(MOS6502_OBJ) : $(MOS6502_PATCHED)
	$(CXX) $(CFLAGS) -c -o $@ $(MOS6502_CXX)

test :
	make -C example

clean :
	-rm -r $(BUILD_DIR)

$(foreach SOURCE,$(SOURCES),$(eval $(subst \,,$(shell $(CXX) $(CFLAGS) -MM $(SOURCE) | sed -e 's/^\([^ ]*\)\.o:/$(BUILD_DIR)\/\1.o:/'))))
