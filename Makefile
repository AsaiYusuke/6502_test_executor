SRC_DIR		:=	src
INC_DIR		:=	$(SRC_DIR)/include

TARGET		:=	6502_tester
SOURCES		:=	$(shell find $(SRC_DIR) -type f -name '*.cpp')
HEADERS		:=	$(shell find $(SRC_DIR) -type f -name '*.h')

ARGS_HEADER	:=	../args/args.hxx
ARGS_URL	:=	https://github.com/Taywee/args.git
JSON_HEADER	:=	../json/single_include/nlohmann/json.hpp
JSON_URL	:=	https://github.com/nlohmann/json.git

CFLAGS		:=	--std=c++11 -I $(INC_DIR) -g

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS) $(ARGS_HEADER) $(JSON_HEADER)
	g++ $(CFLAGS) -o $(TARGET) $(SOURCES)

$(ARGS_HEADER) :
	(cd .. ; git clone $(ARGS_URL))

$(JSON_HEADER) :
	(cd .. ; git clone $(JSON_URL))
