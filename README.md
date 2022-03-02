# 6502 Unit Test executor
This tool enables [unit testing](https://en.wikipedia.org/wiki/Unit_testing) for 6502 assembly programs on a cross-platform basis.

## Getting started
[A simple example project](https://github.com/AsaiYusuke/6502_test_executor/tree/master/example) has everything you need for this tool.


```
# cd example
# make
mkdir -p build
ca65 --cpu 6502 --target nes --debug-info -o build/example.o src/example.asm
mkdir -p dist
ld65  -o dist/example.nes --dbgfile dist/example.dbg --config cfg/nes.cfg --obj build/example.o
../6502_tester -d dist/example.dbg --quiet-ok --quiet-summary -t test/ok/customize.configurations.test.json
../6502_tester -d dist/example.dbg --quiet-ok --quiet-summary -t test/ok/error.timeout.test.json
:
All tests passed.
```

## How to use

### Enable debug option
Build your 6502 project using [CA65 assembler](https://cc65.github.io/doc/ca65.html)/[LD65 linker](https://cc65.github.io/doc/ld65.html) with *debug information generation* enabled.

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
Run the tool with the prepared debug information file and test scenario file:

```
6502_tester -d <debug information> -t <test scinario>
```

You can find all option arguments by displaying the help:

```
# ./6502_tester --help
  ./6502_tester {OPTIONS}

    6502 test executor

  OPTIONS:

      -h, --help                        Show this help menu.
      -d[DEBUG], --debug=[DEBUG]        Specify the path of the debug
                                        information file used for testing.
      -t[TEST], --test=[TEST]           (REQUIRED)
                                        Specify the path of the test scinario
                                        file.
      -i[ID], --id=[ID]                 Specify the test ID.
      --timeout=[TIMEOUT]               Specify the timeout period before the
                                        test becomes an error.
      -q, --quiet                       Do not show any output.
      --quiet-ok                        Do not show the successful output.
      --quiet-fail                      Do not show the failed output.
      --quiet-summary                   Do not show the test summary output.

```

Some options can be specified either as command line arguments or test scenario file.
If both are specified, the values in the test scenario file are adopted.


## Test scinario examples

### Registers
- [Check the value of the A/X/Y registers](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/register.axy.value.test.json)
- [Check the flags of the Processor status register](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/register.status.flag.test.json)

### Memory
- [Memory addressing](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory.addressing.test.json)
- [Check the value of the memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory.value.check.test.json)
- [Check the read/write count](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory.count.check.test.json)
- [Batch selection of continuous memory area](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory.contiguous.memory.area.test.json)
- [Sequential change value](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory.sequential.change.value.test.json)

### Error
- [Timeout](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/error.timeout.test.json)
- [Write to readonly memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/fail/error.readonly.test.json)
- [Access to out of segment](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/fail/error.out_of_segment.test.json)

### Misc
- [Customize configurations](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/customize.configurations.test.json)
- [Skip test (ignore)](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/skip.test.json)

# Dependencies
This project uses following project:
- [Gianluca Ghettini's 6502 emulator](https://github.com/gianlucag/mos6502)
  - [Erik Lothe's 6502 Unit test executor](https://github.com/89erik/6502_test_executor)
- [Taylor C. Richberger's args](https://github.com/Taywee/args)
- [Niels Lohmann's JSON for Modern C++](https://github.com/nlohmann/json)
