SRC_DIR			:=		src
CONFIG_DIR		:=		cfg
BUILD_DIR		:=		build
DIST_DIR		:=		dist
TEST_DIR		:=		test
TEST_OK_DIR		:=		$(TEST_DIR)/ok
TEST_FAIL_DIR	:=		$(TEST_DIR)/fail

PROJECT		:=		example
TARGET		:=		$(DIST_DIR)/$(PROJECT).nes
DEBUG		:=		$(DIST_DIR)/$(PROJECT).dbg
SOURCES		:=		$(shell find $(SRC_DIR) -type f -name '*.asm')
OBJECTS		:=		$(SOURCES:$(SRC_DIR)/%.asm=$(BUILD_DIR)/%.o)
NES_CFG		:=		$(CONFIG_DIR)/nes.cfg

TEST_OK		:=		$(shell find $(TEST_OK_DIR) -type f -name '*.test.json' | sort)
TEST_FAIL	:=		$(shell find $(TEST_FAIL_DIR) -type f -name '*.test.json' | sort)
TEST_IDS	:=		$(TEST_OK:%.test.json=%.test) $(TEST_FAIL:%.test.json=%.test)

AS			:=		ca65
ASFLAGS		:=		--cpu 6502 --target nes --debug-info
LD			:=		ld65
LDFLAGS		:=		
TEST_EXEC	:=		../6502_tester

.PHONY : all build test clean

all : build test

build : $(TARGET)

$(TARGET) : $(OBJECTS) $(NES_CFG)
	mkdir -p $(DIST_DIR)
	$(LD) $(LDFLAGS) -o $(TARGET) --dbgfile $(DEBUG) --config $(NES_CFG) --obj $(OBJECTS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.asm
	mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) -o $@ $<

test : $(TEST_EXEC) $(TEST_IDS)
	@echo "All tests passed."

$(TEST_EXEC) :
	make -C ..

$(TEST_OK_DIR)/%.test : $(TEST_OK_DIR)/%.test.json
	$(TEST_EXEC) -p $(TARGET) -d $(DEBUG) --quiet-ok --quiet-summary -t $<

$(TEST_FAIL_DIR)/%.test : $(TEST_FAIL_DIR)/%.test.json
	-$(TEST_EXEC) -p $(TARGET) -d $(DEBUG) --quiet-ok --quiet-summary -t $<

clean :
	rm -rf $(BUILD_DIR)
