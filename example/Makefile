SRC_DIR		:=		src
CONFIG_DIR	:=		cfg
BUILD_DIR	:=		build
DIST_DIR	:=		dist
TEST_DIR	:=		test

PROJECT		:=		example
TARGET		:=		$(DIST_DIR)/$(PROJECT).nes
DEBUG		:=		$(DIST_DIR)/$(PROJECT).dbg
SOURCES		:=		$(shell find $(SRC_DIR) -type f -name '*.asm')
OBJECTS		:=		$(SOURCES:$(SRC_DIR)/%.asm=$(BUILD_DIR)/%.o)
NES_CFG		:=		$(CONFIG_DIR)/nes.cfg

TESTS		:=		$(shell find $(TEST_DIR) -type f -name '*.test.json' | sort)
TEST_IDS	:=		$(TESTS:%.test.json=%.test)

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

%.test : %.test.json
	$(TEST_EXEC) -p $(TARGET) -d $(DEBUG) --quiet-ok --quiet-summary -t $<

clean :
	rm -rf $(BUILD_DIR)
