# 6502 Unit Test executor
This tool enables unit testing for 6502 assembly programs on a cross-platform basis. 

This project is a fork of the following project:
- [gianlucag's 6502 emulator](https://github.com/gianlucag/mos6502)
  - [Erik Lothe's 6502 Unit test executor](https://github.com/89erik/6502_test_executor)

## Getting started
[A simple example project](https://github.com/AsaiYusuke/6502_test_executor/tree/master/example) that gives you everything you need for this tool is available.

```
# cd example
# make
mkdir -p build
ca65 --cpu 6502 --target nes --debug-info -o build/example.o src/example.asm
mkdir -p dist
ld65  -o dist/example.nes --dbgfile dist/example.dbg --config cfg/nes.cfg --obj build/example.o
../6502_tester -p dist/example.nes -d dist/example.dbg --quiet-ok --quiet-summary -t test/ex.customize.configurations.test.json
../6502_tester -p dist/example.nes -d dist/example.dbg --quiet-ok --quiet-summary -t test/ex.error.timeout.test.json
../6502_tester -p dist/example.nes -d dist/example.dbg --quiet-ok --quiet-summary -t test/ex.memory.addressing.test.json
../6502_tester -p dist/example.nes -d dist/example.dbg --quiet-ok --quiet-summary -t test/ex.memory.contiguous.memory.area.test.json
../6502_tester -p dist/example.nes -d dist/example.dbg --quiet-ok --quiet-summary -t test/ex.memory.count.check.test.json
../6502_tester -p dist/example.nes -d dist/example.dbg --quiet-ok --quiet-summary -t test/ex.memory.sequential.change.value.test.json
../6502_tester -p dist/example.nes -d dist/example.dbg --quiet-ok --quiet-summary -t test/ex.memory.value.check.test.json
../6502_tester -p dist/example.nes -d dist/example.dbg --quiet-ok --quiet-summary -t test/ex.register.axy.value.test.json
../6502_tester -p dist/example.nes -d dist/example.dbg --quiet-ok --quiet-summary -t test/ex.register.status.flag.test.json
echo "All tests passed."
All tests passed.
```

## How to use

### (Optional) Build the 6502 project with debug option
Build the 6502 project using the [CA65 assembler](https://cc65.github.io/doc/ca65.html) with debug option.
By specifying the debug option, symbol definitions consisting of label/address pair will be output.

### Create test scinario files
Create test scinario files containing the three key items in JSON format:

- *Test target*  
the starting address of the test procedure
- *Setup condition*  
the settings of the register and memory to make before the test
- *Expected condition*  
the expected responses of the register and memory after the test

#### [JSON Schema file](https://github.com/AsaiYusuke/6502_test_executor/blob/master/schema/testcase.schema.json)
The tool also provides a [JSON Schema](https://json-schema.org/) document that makes it easy to create test scinario files.
If you use [Visual Studio Code](https://code.visualstudio.com/), it will tell you about formatting error based on JSON Schema without any extensions.

### Execute tester
Run the tool by specifying the program image file, debug information file, and test scinario file:

```
6502_tester -p <program iamge> -d <debug information> -t <test scinario>
```

You can check the available optional arguments by displaying the help:

```
# ./6502_tester --help
  ./6502_tester {OPTIONS}

    6502 test executor

  OPTIONS:

      -h, --help                        Show this help menu.
      -p[PROGRAM], --program=[PROGRAM]  Specify the path of the program image
                                        file to be tested.
      -d[DEBUG], --debug=[DEBUG]        Specify the path of the debug
                                        information file used for testing.
      -t[TEST], --test=[TEST]           (REQUIRED)
                                        Specify the path of the test scinario
                                        file.
      --timeout=[TIMEOUT]               Specify the timeout period before the
                                        test becomes an error.
      -q, --quiet                       Do not show any output.
      --quiet-ok                        Do not show the successful output.
      --quiet-fail                      Do not show the failed output.
      --quiet-summary                   Do not show the test summary output.

```

## Test scinario examples

### Registers
- [Check the value of the A/X/Y registers](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ex.register.axy.value.test.json)
- [Check the flags of the Processor status register](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ex.register.status.flag.test.json)

### Memory
- [Memory addressing](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ex.memory.addressing.test.json)
- [Check the value of the memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ex.memory.value.check.test.json)
- [Check the read/write count](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ex.memory.count.check.test.json)
- [Check the value of the continuous memory area](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ex.memory.contiguous.memory.area.test.json)
- [Sequential change value](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ex.memory.sequential.change.value.test.json)

### Error
- [Check the Timeout error](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ex.error.timeout.test.json)

### Misc
- [Customize configurations](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ex.customize.configurations.test.json)
- [Skip test (ignore)](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ex.skip.test.json)
