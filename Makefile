all: main.cpp mos6502.cpp
	c++ --std=c++11 main.cpp mos6502.cpp -o 6502_tester
