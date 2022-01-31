# 6502 Unit Test executor
This tool enables unit testing for 6502 assembly programs built with the [ca65 assembler](https://cc65.github.io/doc/ca65.html) on a cross-platform basis.

#### Note:
This project is a fork of [Erik Lothe's 6502 Unit test executor](https://github.com/89erik/6502_test_executor) contains [gianlucag's 6502 emulator](https://github.com/gianlucag/mos6502).

## Getting started
A simple project that gives you everything you need for this tool is available in `example` directory.

```
# cd example
# make
mkdir -p build
ca65 --cpu 6502 --target nes --debug-info -o build/example.o src/example.asm
mkdir -p dist
ld65  -o dist/example.nes --dbgfile dist/example.dbg --config cfg/nes.cfg --obj build/example.o
../6502_tester -p dist/example.nes -s dist/example.dbg --quiet-ok --quiet-summary -t test/check_collision.test.json
../6502_tester -p dist/example.nes -s dist/example.dbg --quiet-ok --quiet-summary -t test/control.test.json
echo "All tests passed."
All tests passed.
```

## How to use

### Build the 6502 project with debug option
By specifying the debug option to the ca65/ld65 command, symbol definitions consisting of label/address pair will be output.

### Create test files
Create test files consisting of three items in JSON format:
- the starting address of the procedure to be tested
- the register and memory settings to be made before execution
- the expected values of the register and memory after the test procedure is executed

And also the tool provides a `JSON Schema file` for creating test files, so you can easily write tests that satisfy the format.
If you use [Visual Studio Code](https://code.visualstudio.com/), it will tell you about formatting error based on JSON Schema without any extensions.

### Execute tester
Run the tester by specifying the program image file, symbol definition file, and test file:

```
6502_tester -p <program iamge> -s <symbol definition> -t <test file>
```

You can check the available optional arguments by displaying the help:

```
# ./6502_tester --help
  ./6502_tester {OPTIONS}

    6502 test executor

  OPTIONS:

      -h, --help                        Show this help menu.
      -p[PROGRAM], --program=[PROGRAM]  The program file path.
      -s[SYMBOL], --symbol=[SYMBOL]     The symbol file path.
      -t[TEST], --test=[TEST]           The test file path.
      --quiet-ok                        The quiet OK flag
      --quiet-fail                      The quiet FAIL flag
      --quiet-summary                   The quiet summary flag
      -q, --quiet                       The quiet flag

```