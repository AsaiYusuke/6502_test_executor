##MOS6502 Unit Test executor ##
This repository is a fork of gianlucag's MOS6502 emulator, and adds a driver for performing unit tests on it. 

##How to use##
Build with make and start the driver (6502_tester) with the following arguments:

### Argument 1: 6502 executable ###
Path to the 6502 program that the test should be performed on.

### Argument 2: symbols table ###
Path to the symbols table. This file contains debug information from the building of the 6502 program. To generate this file when building with the cc65 compiler, use the --debug-info flag when compiling and --dbgfile <filename> when linking.

### Argument 3: test procedure ###
Path to the test procedure file. This file contains a description for performing the test, as this example file illustrates:

start TestMultiply<br />
set A 4<br />
set X 3<br />
assert A 12<br />

This will start by setting the A register to 4 and X to 3, then the PC to TestMultiply. The latter address is determined by the symbols table. TestMultiply is assumed to point to a JSR instruction and the test terminates once this JSR has returned. Finally it will check if the A register is 12, and if so the test will have succeeded.
