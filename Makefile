SRC_DIR			:=	src
INC_DIR			:=	include
BUILD_DIR		:=	build

SRC_DIRS		:=	$(shell find $(SRC_DIR) -type d)
BUILD_DIRS		:=	$(SRC_DIRS:$(SRC_DIR)%=$(BUILD_DIR)%)

TARGET			:=	6502_tester
SOURCES			:=	$(shell find $(SRC_DIR) -type f -name '*.cpp')
HEADERS			:=	$(shell find $(SRC_DIR) -type f -name '*.h')
OBJECTS         :=	$(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

ARGS_INC_DIR	:=	../args
ARGS_HEADER		:=	$(ARGS_INC_DIR)/args.hxx
ARGS_URL		:=	https://github.com/Taywee/args.git

JSON_INC_DIR	:=	../json/single_include
JSON_HEADER		:=	$(JSON_INC_DIR)/nlohmann/json.hpp
JSON_URL		:=	https://github.com/nlohmann/json.git

CFLAGS			:=	--std=c++11 -I $(INC_DIR) -I $(ARGS_INC_DIR) -I $(JSON_INC_DIR) -g

.PHONY : all clean

all: $(BUILD_DIRS) $(TARGET)

$(BUILD_DIRS) : % :
	mkdir -p $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CFLAGS) -o $(TARGET) $(OBJECTS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(ARGS_HEADER) :
	(cd .. ; git clone $(ARGS_URL))

$(JSON_HEADER) :
	(cd .. ; git clone $(JSON_URL))

clean :
	rm -rf $(BUILD_DIR)
	rm $(TARGET)

$(foreach SOURCE,$(SOURCES),$(eval $(subst \,,$(shell $(CXX) $(CFLAGS) -MM $(SOURCE) | sed -e 's/^\([^ ]*\)\.o:/$(BUILD_DIR)\/\1.o:/'))))
