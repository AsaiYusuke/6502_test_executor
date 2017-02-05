##MOS6502 Unit Test executor ##
This repository is a fork of gianlucag's MOS6502 emulator, and adds a driver for performing unit tests on it. 

##How to use##
Build with make and start the driver (6502_tester) with the following arguments:

### Argument 1: 6502 executable ###
Path to the 6502 program that the test should be performed on.

### Argument 2: symbols table ###
Path to the symbols table. This file contains debug information from the building of the 6502 program. To generate this file when building with the cc65 compiler, use the --debug-info flag when compiling and --dbgfile <filename> when linking.

### Argument 3: test procedure ###
Path to the test procedure file. This file contains a description for performing the test, using three commands "set", "start" and "assert". Note that the order of these commands does not matter: It will always perform all "set"-commands before starting the test, and always perform asserts after the return of the function under test.

#### set
The set instruction is used to set the state of the machine before starting to run the code. It can be used in the following formats:
```
set <register> <value>
set mem <label>([<offset>]) <value>
```

In both examples value should be a number. It can be specified in the following ways: 
* Signed base 10 (decimal), e.g. 123 or -10 (onehundred twenty three or minus ten)
* Unsigned base 16 (hexadecimal), e.g. 0xff, 0xFF or 0x1 (twohundred fifty five, twohundred fifty five or one)
* Unsigned base 2 (binary), e.g. 0b10 (two)

In the first example, <register> can be either A, X or Y, refering to the corresponding CPU registers. The second example refers to memory locations given by a label in the source such as player_position. The memory address is determined by the symbols table. An optional offset can be added using C-style array-offset notation, e.g. my_array[3].

#### start
The start command is used to set the PC register, meaning telling the processor where to start when executing the test. This memory address is given by a single argument containing a label, just like in the set mem instruction. Note that the test executor will stop upon reaching the instruction directly after the one given by the start command. This means that for the test to make sense, the start command must point to a JSR instruction. The test will then terminate when this JSR returns. Also note that the function that the JSR instruction is pointing to cannot be directly below the JSR. This is because it would then jump only to the next instruction, and the test would immediately terminate.

#### assert
The assert command is identical to the set command, except that "set" is replaced with "assert". In this case the memory address or register is checked to be equal to the given value. If all assert commands holds true, the test will pass.

#### Test file examples
```
set A 4
set X 3
start TestMultiply
assert A 12
```

This will start by setting the A register to 4 and X to 3, then the PC to the address of the TestMultiply label. Finally it will check if the A register is 12, and if so the test will have succeeded. 

It is also possible to refer to variables such as in the following, assuming player_position is a label in the symbols table:

```
set mem player_position 10
start TestMovePlayer
assert player_position 11
```
