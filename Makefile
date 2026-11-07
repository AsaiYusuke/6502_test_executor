SRC_DIR				:=	src
INC_DIR				:=	include
BUILD_DIR			:=	build
THIRD_PARTY_DIR		:=	third_party

TARGET				:=	6502_tester
SOURCES				:=	$(shell find $(SRC_DIR) -type f -name '*.cpp' | sort)
OBJECTS				:=	$(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS				:=	$(OBJECTS:.o=.d)

ARGS_DIR			:=	$(THIRD_PARTY_DIR)/args

JSON_DIR			:=	$(THIRD_PARTY_DIR)/json/single_include

MOS6502_DIR			:=	$(THIRD_PARTY_DIR)/mos6502
MOS6502_CXX			:=	$(MOS6502_DIR)/mos6502.cpp
MOS6502_OBJ			:=	$(BUILD_DIR)/mos6502.o
DEPS				+=	$(MOS6502_OBJ:.o=.d)

CPPFLAGS			+=	-I$(INC_DIR) -I$(ARGS_DIR) -I$(JSON_DIR) -I$(MOS6502_DIR)
CXXFLAGS			?=	-g
PROJECT_CXXFLAGS		:=	-std=c++20
BUILD_FLAGS			:=	$(CPPFLAGS) $(CXXFLAGS) $(PROJECT_CXXFLAGS)

.PHONY : all exec clean test compile_commands

all: exec
	$(MAKE) test

exec: compile_commands $(TARGET)

compile_commands:
	python3 tools/generate_compile_commands.py "$(CURDIR)" "$(BUILD_DIR)" "$(CXX)" "$(BUILD_FLAGS)" "$(SOURCES)" "$(MOS6502_CXX)" "$(MOS6502_OBJ)"

$(TARGET) : $(MOS6502_OBJ) $(OBJECTS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(PROJECT_CXXFLAGS) -o $@ $(OBJECTS) $(MOS6502_OBJ) $(LDLIBS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(BUILD_FLAGS) -MMD -MP -MF "$(@:.o=.d)" -MT "$@" -c -o $@ $<

$(MOS6502_OBJ) : $(MOS6502_CXX)
	mkdir -p $(@D)
	$(CXX) $(BUILD_FLAGS) -MMD -MP -MF "$(@:.o=.d)" -MT "$@" -c -o $@ $<

test : $(TARGET)
	python3 tools/check_mos6502_opcode_metadata.py
	python3 tools/check_rom_image_validation.py
	$(MAKE) -C example

clean :
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)
