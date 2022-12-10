SRC_DIR			:=		src
CONFIG_DIR		:=		cfg
BUILD_DIR		:=		build
DIST_DIR		:=		dist
TEST_DIR		:=		test
TEST_OK_DIR		:=		$(TEST_DIR)/ok
TEST_FAIL_DIR		:=		$(TEST_DIR)/fail
TEST_EXEC_DIR		:=		..
COVERAGE_DIR		:=		coverage

PROJECT			:=		example
TARGET			:=		$(DIST_DIR)/$(PROJECT).nes
DEBUG			:=		$(DIST_DIR)/$(PROJECT).dbg
SOURCES			:=		$(shell find $(SRC_DIR) -type f -name '*.asm')
OBJECTS			:=		$(SOURCES:$(SRC_DIR)/%.asm=$(BUILD_DIR)/%.o)
NES_CFG			:=		$(CONFIG_DIR)/nes.cfg

TEST_OK			:=		$(shell find $(TEST_OK_DIR) -type f -name '*.test.json' | sort)
TEST_FAIL		:=		$(shell find $(TEST_FAIL_DIR) -type f -name '*.test.json' | sort)
TEST_IDS		:=		$(TEST_OK:%.test.json=%.test) $(TEST_FAIL:%.test.json=%.test)

COVARAGE		:=		$(COVERAGE_DIR)/lcov.info
COVERAGE_SEGMENTS	:=		"CODE"

AS			:=		ca65
ASFLAGS			:=		--cpu 6502 --target nes --debug-info
LD			:=		ld65
LDFLAGS			:=		
TEST_EXEC		:=		$(TEST_EXEC_DIR)/6502_tester
TEST_FAILED_CODE	:=		200

TEST_FLAGS		:=		--debug=$(DEBUG) --coverage=$(COVARAGE) --segment=$(COVERAGE_SEGMENTS)
TEST_OK_FLAGS		:=		--quiet-summary --quiet-ok
TEST_FAIL_FLAGS		:=		--quiet-summary --quiet-fail

.PHONY : all build test prepare clean

all : build test

build : $(TARGET)

$(TARGET) : $(OBJECTS) $(NES_CFG)
	mkdir -p $(DIST_DIR)
	$(LD) $(LDFLAGS) -o $(TARGET) --dbgfile $(DEBUG) --config $(NES_CFG) --obj $(OBJECTS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.asm
	mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) -o $@ $<

test : $(TEST_EXEC) test_prepare $(TEST_IDS)
	@echo "All tests passed."

$(TEST_EXEC) :
	make -C $(TEST_EXEC_DIR)

test_prepare : $(COVERAGE_DIR)
	-rm $(COVARAGE)

$(COVERAGE_DIR) :
	mkdir -p $(COVERAGE_DIR)

$(TEST_OK_DIR)/%.test : $(TEST_OK_DIR)/%.test.json
	$(TEST_EXEC) $(TEST_FLAGS) $(TEST_OK_FLAGS) -t $<

$(TEST_FAIL_DIR)/%.test : $(TEST_FAIL_DIR)/%.test.json
	$(TEST_EXEC) $(TEST_FLAGS) $(TEST_FAIL_FLAGS) -t $< && { exit 1; } || { [ $$? -eq $(TEST_FAILED_CODE) ] && exit 0 || exit 1; }

clean :
	-rm -r $(BUILD_DIR)
