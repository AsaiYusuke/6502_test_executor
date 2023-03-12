# 6502 Unit Test executor

[![example workflow](https://github.com/AsaiYusuke/6502_test_executor/actions/workflows/make.yml/badge.svg)](https://github.com/AsaiYusuke/6502_test_executor/actions/workflows/make.yml)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/4c943cf3172642469b3249fbc6f4c672)](https://www.codacy.com/gh/AsaiYusuke/6502_test_executor/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=AsaiYusuke/6502_test_executor&amp;utm_campaign=Badge_Grade)
[![Awesome](https://cdn.rawgit.com/sindresorhus/awesome/d7305f38d29fed78fa85652e3a63e154dd8e8829/media/badge.svg)](https://github.com/ellisonleao/magictools#engines-and-frameworks)

![6502 unit test](6502-unit-test.svg)

This tool enables [unit testing](https://en.wikipedia.org/wiki/Unit_testing) for [MOS Technology 6502](https://en.wikipedia.org/wiki/MOS_Technology_6502) assembly programs on a cross-platform basis.

## Table of Contents

- [Basic design](#Basic-design)
- [Supported testing feature](#Supported-testing-feature)
  - [Testable program](#Testable-program)
  - [Condition](#Condition)
  - [Assertion](#Assertion)
  - [Coverage](#Coverage)
  - [Additional useful feature](#Additional-useful-feature)
- [Getting started](#getting-started)
  - [Prerequisites](#Prerequisites)
  - [Install](#Install)
  - [Test example project](#Test-example-project)
    - [Unit testing](#Unit-testing)
    - [Coverage](#Coverage)
- [Usage](#Usage)
  - [Build CA65 project with debug option](#Build-CA65-project-with-debug-option)
  - [Create unit test](#Create-unit-test)
  - [Run test](#Run-test)
- [Test scenario examples](#Test-scenario-examples)
- [Dependencies](#Dependencies)

## Basic design

#### _Execute test on built-in emulator_

- Built-in [6502 emulation by Gianluca Ghettini](https://github.com/gianlucag/mos6502) enables stand-alone testing on Linux, etc.
- No need to add hooks to the product code to invoke test.

#### _Write test with JSON Schema_

- The tool provides [JSON Schema](https://json-schema.org/) document that makes it easy to create test.

#### _Evaluate test like modern frameworks_

- It comes with many useful evaluation methods found in [the modern UNIT testing frameworks](https://en.wikipedia.org/wiki/List_of_unit_testing_frameworks).

## Supported testing features

### Testable program

- Test target
  - JSR procedure
  - NMI procedure
  - IRQ procedure
  - Address range
- Type of interrupts that can occur during testing
  - NMI
  - IRQ
- Mockable call instructions that set the result in register and memory
  - JSR
  - JMP

### Condition

- Setting up device conditions before testing  
  | | Register | Memory | Stack |
  | :---------------------- | :----------------: | :----------------: | :----------------: |
  | Set value | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
  | Mock read value history | | :heavy_check_mark: | |
- Evaluating device conditions after testing  
  | | Register | Memory | Stack |
  | :------------------------ | :----------------: | :----------------: | :----------------: |
  | Check value | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
  | Check read/write count | :heavy_check_mark: | :heavy_check_mark: | |
  | Check write value history | | :heavy_check_mark: | |
- Evaluating processor information
  - Cycle count

### Assertion

- Assertion operators that determine how tests is evaluated
  - _"eq"_ (Equal to)
  - _"ne"_ (Not equal to)
  - _"gt"_ (Greater than)
  - _"ge"_ (Greater than or equal to)
  - _"lt"_ (Less than)
  - _"le"_ (Less than or equal to)
  - _"anyOf"_ (Any of)  
    Representing logical OR operator
  - Composite operators  
    Same as logical AND operator. e.g. "_gt_"+"_lt_" for range selection
- Error handling
  - Write access to readonly memory  
    Able to detect write operations to readonly memory.
  - Read access from uninitialized memory  
    Able to detect read operations from memory that has never had a write operation.
  - Access to undefined memory  
    Able to detect memory operations using addresses outside the segment defined at build time.
  - Access to unauthorized memory  
    Able to detect access outside the memory for which authorized area has been defined.
  - Illegal instruction  
    Able to detect illegal instruction code execution.

### Coverage

- Testable coverage
  - [LCOV](https://github.com/linux-test-project/lcov) format

### Test classification
- Aggregated testing for the entire project
- Common configuration settings for the entire project
- Grouping of individual tests

### Additional useful feature

- When a test terminates with an error, its callback trace can be output.
- Test evaluations can be executed at any address.
- Test ROM images are detected from debug information file.

## Getting started

### Prerequisites

This tool is intended for the projects based on [CC65](https://cc65.github.io/).

The easiest way to install CC65 on [Ubuntu](https://ubuntu.com/) linux is by running:

```
# sudo apt-get install cc65
```

Verified CA65 version:

```
# ca65 --version
ca65 V2.18 - Ubuntu 2.19-1
```

### Install

Since the tool repository has the submodules, the `--recurse-submodules` option must be specified when cloning:

```
# git clone --recurse-submodules https://github.com/AsaiYusuke/6502_test_executor.git
# cd 6502_test_executor
# make
# ./6502_tester --help
```

### Run example

#### Unit testing

[Simple example project](https://github.com/AsaiYusuke/6502_test_executor/tree/master/example) includes many test cases that demonstrate the features:

```
# cd example
# make
mkdir -p build
ca65 --cpu 6502 --target nes --debug-info -o build/example.o src/example.asm
mkdir -p dist
ld65  -o dist/example.nes --dbgfile dist/example.dbg --config cfg/nes.cfg --obj build/example.o
rm coverage/lcov.info
../6502_tester --debug=dist/example.dbg --coverage=coverage/lcov.info --segment="CODE" --quiet-summary --quiet-ok -t test/ok/customize.configurations.test.json
../6502_tester --debug=dist/example.dbg --coverage=coverage/lcov.info --segment="CODE" --quiet-summary --quiet-ok -t test/ok/definitions.test.json
../6502_tester --debug=dist/example.dbg --coverage=coverage/lcov.info --segment="CODE" --quiet-summary --quiet-ok -t test/ok/error.timeout.test.json
:
All tests passed.
```

#### Coverage

When the project is built, the coverage file is saved.
It is located on `example/coverage/lcov.info` in case of example project.

The coverage file can be used to populate [Coveralls GitHub Action](https://github.com/marketplace/actions/coveralls-github-action), etc.

The results of the example project can be seen in [Coveralls](https://coveralls.io/github/AsaiYusuke/6502_test_executor).\
[![Coverage Status](https://coveralls.io/repos/github/AsaiYusuke/6502_test_executor/badge.svg?branch=master)](https://coveralls.io/github/AsaiYusuke/6502_test_executor?branch=master)

## Usage

```mermaid
flowchart LR;
  A(Assembly program);
  C[[CA65]];
  B(Binary);
  D(Debug information);
  V[[Visual Studio Code]];
  S(Test scenario);
  U[[6502 Unit Test executor]];
  R[(Test result)];
  L[(Test coverage)];

  subgraph Build CA65 project
    A --> C --> B & D;
  end

  subgraph Create unit test
    V -- Json schema --> S;
  end

  subgraph Run test
    B & D & S --> U --> R & L;
  end
```

### Build CA65 project with debug option

Build your 6502 project using [CA65 assembler](https://cc65.github.io/doc/ca65.html)/[LD65 linker](https://cc65.github.io/doc/ld65.html) with _debug information generation_ enabled.

### Create unit test

Create test scenario files containing the three key items in JSON format:

- _Test target_  
  the starting address of the test procedure
- _Setup condition_  
  the settings of the register and memory to make before the test
- _Expected condition_  
  the expected responses of the register and memory after the test

#### JSON Schema file

The tool also provides a [JSON Schema](https://json-schema.org/) document that makes it easy to create test scenario files.

If you use [Visual Studio Code](https://code.visualstudio.com/), it will tell you about formatting error and element completion candidates based on JSON Schema without any extensions.

### Run test

Run the tool with the prepared debug information file and test scenario file:

```
6502_tester -d <debug information> -t <test scenario>
```

Test coverage can also be measured.
Both the coverage file and the segment names used in the debug information file must be specified to enable.

```
6502_tester -d <debug information> -t <test scenario> -c <coverage> -s <segment names>
```

You can find all command line arguments in help:

```
# ./6502_tester --help
  ./6502_tester {OPTIONS}

    6502 test executor

  OPTIONS:

      -h, --help                        Show this help menu.
      -d[DEBUG], --debug=[DEBUG]        Specify the path of the debug
                                        information file used for testing.
      -t[TEST], --test=[TEST]           (REQUIRED)
                                        Specify the path of the test scenario
                                        file.
      -c[COVERAGE],
      --coverage=[COVERAGE]             Specify the path of the coverage file.
      -s[SEGMENT], --segment=[SEGMENT]  Specify the segment names for coverage.
      -i[ID], --id=[ID]                 Specify the test ID.
      --timeout=[TIMEOUT]               Specify the timeout period before the
                                        test becomes an error.
      -q, --quiet                       Do not show any output.
      --quiet-ok                        Do not show the successful output.
      --quiet-fail                      Do not show the failed output.
      --quiet-summary                   Do not show the test summary output.

```

### Note:

Some options can be specified either as command line arguments or test scenario file ([See example](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/customize.configurations.test.json)).  
If both are specified, the values in the test scenario file are adopted.

## Test scenario examples

### Register conditions

- [Check value of A/X/Y registers](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/register.axy.value.test.json)
- [Check read/write count of A/X/Y registers](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/register.axy.count.test.json)
- [Check value of Processor status register flags](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/register.status.flag.value.test.json)
- [Check read/write count of Processor status register flags](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/register.status.flag.count.test.json)
- [Check machine when PC register arrives at a specific address](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/init.code.test.json)

### Memory conditions

- [Memory addressing](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory.addressing.test.json)
- [Check value of memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory.value.check.test.json)
- [Check read/write count of memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory.count.check.test.json)
- [Batch selection of continuous memory area](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory.contiguous.memory.area.test.json)
- [Sequential change value](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory.sequential.change.value.test.json)

### Stack conditions

- [Check value of stack](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/stack.value.check.test.json)
- [rts to caller](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/stack.rts.check.test.json)

### Processor conditions

- [Cycle count information](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/error.timeout.test.json)
- [Interrupt hooks](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/interrupt.test.json)
- [Call mocked proc](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/mock.proc.test.json)

### Testable procedures

- [JSR proc](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/test_type/jsr.test.json)
- [NMI proc](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/test_type/nmi.test.json)
- [IRQ proc](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/test_type/irq.test.json)
- [Address range](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/test_type/address.test.json)

### Error handling

- [Write access to readonly memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/fail/200/error.readonly.test.json)
- [Read access from uninitialized memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/fail/200/error.uninitialized_memory_read.test.json)
- [Access to undefined memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/fail/200/error.out_of_segment.test.json)
- [Access to unauthorized memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/fail/200/error.unauthorized_memory_access.test.json)
- [Illegal instruction](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/fail/200/error.illegal_instruction.test.json)

### Test classification

- [Aggregated testing](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/project/project.test.json)
- [Common configuration settings](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/fail/200/project/error.out_of_segment.test.json)
  - [Override from project-wide configuration settings](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/project/merge_config.test.json)
- [Grouping of individual tests](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/group.test.json)

### Misc

- [Customize configurations](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/customize.configurations.test.json)
- [Skip test (ignore)](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/skip.test.json)
- [Complex evaluation](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/evaluation.test.json)
- [Definitions](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/definitions.test.json)

## Dependencies

This project uses following project:

- [Gianluca Ghettini's 6502 emulator](https://github.com/gianlucag/mos6502)
  - [Erik Lothe's 6502 Unit test executor](https://github.com/89erik/6502_test_executor)
- [Taylor C. Richberger's args](https://github.com/Taywee/args)
- [Niels Lohmann's JSON for Modern C++](https://github.com/nlohmann/json)

## License

This project is available under the **MIT license**. See the [LICENSE](LICENSE) file for more information.
