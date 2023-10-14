# 6502 Unit Test executor

[![example workflow](https://github.com/AsaiYusuke/6502_test_executor/actions/workflows/make.yml/badge.svg)](https://github.com/AsaiYusuke/6502_test_executor/actions/workflows/make.yml)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/4c943cf3172642469b3249fbc6f4c672)](https://www.codacy.com/gh/AsaiYusuke/6502_test_executor/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=AsaiYusuke/6502_test_executor&amp;utm_campaign=Badge_Grade)
[![Awesome](https://cdn.rawgit.com/sindresorhus/awesome/d7305f38d29fed78fa85652e3a63e154dd8e8829/media/badge.svg)](https://github.com/ellisonleao/magictools#engines-and-frameworks)

![6502 unit test](6502-unit-test.svg)

This tool enables [unit testing](https://en.wikipedia.org/wiki/Unit_testing) for [MOS Technology 6502](https://en.wikipedia.org/wiki/MOS_Technology_6502) assembly programs on a cross-platform basis.

## Table of Contents

- [Basic Design](#basic-design)
- [Supported Testing Features](#supported-testing-features)
  - [Testable Program](#testable-program)
  - [Conditions](#conditions)
  - [Assertions](#assertions)
  - [Coverage](#coverage)
  - [Test Classification](#test-classification)
  - [Additional Useful Features](#additional-useful-features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Install](#install)
  - [Running Examples](#running-examples)
    - [Unit Testing](#unit-testing)
    - [Coverage File](#coverage-file)
- [Usage](#usage)
  - [Build CA65 Project with Debug Option](#build-ca65-project-with-debug-option)
  - [Create Unit Test](#create-unit-test)
  - [Run Test](#run-test)
- [Test Scenario Examples](#test-scenario-examples)
- [Dependencies](#dependencies)
- [License](#license)

## Basic Design

### *Execute Tests on Built-In Emulator*

- The built-in [6502 emulation by Gianluca Ghettini](https://github.com/gianlucag/mos6502) enables stand-alone testing on platforms such as Linux.
- There's no need to add hooks to the product code to invoke tests.

### *Write Tests with JSON Schema*

- The tool provides [JSON Schema](https://json-schema.org/) document that simplifies test creation.

### *Evaluate Tests like Modern Frameworks*

- It includes many useful evaluation methods found in [the modern UNIT testing frameworks](https://en.wikipedia.org/wiki/List_of_unit_testing_frameworks).

## Supported Testing Features

### Testable Program

- Test targets
  - JSR procedure
  - NMI procedure
  - IRQ procedure
  - Address range
- Type of interrupts that can occur during testing
  - NMI
  - IRQ
- Mockable call instructions that set the results in registers and memory
  - JSR
  - JMP

### Conditions

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

### Assertions

- Assertion operators that determine how tests are evaluated
  - *"eq"* (Equal to)
  - *"ne"* (Not equal to)
  - *"gt"* (Greater than)
  - *"ge"* (Greater than or equal to)
  - *"lt"* (Less than)
  - *"le"* (Less than or equal to)
  - *"anyOf"* (Any of)  
    Representing logical OR operator
  - Composite operators  
    Same as logical AND operator. e.g. "*gt*"+"*lt*" for range selection
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

### Test Classification
- Aggregating tests for the entire project
- Common configuration settings for the entire project
- Grouping of individual tests

### Additional Useful Features

- When a test terminates with an error, its callback trace can be output.
- Test evaluations can be executed at any memory address.
- Test ROM images are detected from debug information file.

## Getting Started

### Prerequisites

This tool is intended for the projects based on `[CC65](https://cc65.github.io/)`.

The easiest way to install `CC65` on `[Ubuntu](https://ubuntu.com/)` linux is by running:

```console
# sudo apt-get install cc65
```

Verified `CA65` version:

```console
# ca65 --version
ca65 V2.18 - Ubuntu 2.19-1
```

### Install

Since this tool repository includes submodules, you must specify the `--recurse-submodules` option when cloning:

```console
# git clone --recurse-submodules https://github.com/AsaiYusuke/6502_test_executor.git
# cd 6502_test_executor
# make
# ./6502_tester --help
```

### Running Examples

#### Unit Testing

The [simple example project](https://github.com/AsaiYusuke/6502_test_executor/tree/master/example) includes many test cases that demonstrate the features:

```console
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

#### Coverage File

When the project is built, the coverage file is saved in the `example/coverage/lcov.info` in the case of the example project.

The coverage file can be used to integrate with tools like [Coveralls GitHub Action](https://github.com/marketplace/actions/coveralls-github-action), and more.

The results of the example project can be seen on [Coveralls](https://coveralls.io/github/AsaiYusuke/6502_test_executor).\
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

### Build CA65 Project with Debug Option

Build your 6502 project using the [CA65 assembler](https://cc65.github.io/doc/ca65.html) and [LD65 linker](https://cc65.github.io/doc/ld65.html) with *debug information generation* enabled.

### Create Unit Test

Create test scenario files containing three key items in JSON format:

- *Test target*  
  The starting address of the test procedure
- *Setup condition*  
  The settings of the registers and memory before the test
- *Expected condition*  
  The expected responses of the registers and memory after the test

#### JSON Schema File

The tool also provides a [JSON Schema](https://json-schema.org/) document that simplifies creating test scenario files.

If you use [Visual Studio Code](https://code.visualstudio.com/), it will provide formatting error notifications and auto-completion based on JSON Schema.

### Run Test

Run the tool with the prepared debug information file and test scenario file:

```text
6502_tester -d <debug information> -t <test scenario>
```

Test coverage can also be measured.
Both the coverage file and the segment names used in the debug information file must be specified to enable coverage reporting.

```text
6502_tester -d <debug information> -t <test scenario> -c <coverage> -s <segment names>
```

You can find all command-line arguments in the help:

```console
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

Some options can be specified either as command-line arguments or test scenario file ([See example](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/customize.configurations.test.json)).  
If both are specified, the values in the test scenario file take precedence.

## Test Scenario Examples

### Register Conditions

- [Check value of A/X/Y registers](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/register/register.axy.value.test.json)
- [Check read/write count of A/X/Y registers](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/register/register.axy.count.test.json)
- [Check value of Processor status register flags](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/register/register.status.flag.value.test.json)
- [Check read/write count of Processor status register flags](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/register/register.status.flag.count.test.json)
- [Check machine when PC register arrives at a specific address](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/register/init.code.test.json)

### Memory Conditions

- [Memory addressing](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory/memory.addressing.test.json)
- [Check value of memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory/memory.value.check.test.json)
- [Check read/write count of memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory/memory.count.check.test.json)
- [Batch selection of continuous memory area](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory/memory.contiguous.memory.area.test.json)
- [Sequential change value](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/memory/memory.sequential.change.value.test.json)

### Stack Conditions

- [Check value of stack](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/stack/stack.value.check.test.json)
- [rts to caller](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/stack/stack.rts.check.test.json)

### Processor Conditions

- [Cycle count information](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/processor/error.timeout.test.json)
- [Interrupt hooks](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/processor/interrupt.test.json)
- [Call mocked proc](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/processor/mock.proc.test.json)

### Testable Procedures

- [JSR proc](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/test_type/jsr.test.json)
- [NMI proc](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/test_type/nmi.test.json)
- [IRQ proc](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/test_type/irq.test.json)
- [Address range](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/test_type/address.test.json)

### Error Handling

- [Write access to readonly memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/fail/200/error.readonly.test.json)
- [Read access from uninitialized memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/fail/200/error.uninitialized_memory_read.test.json)
- [Access to undefined memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/fail/200/error.out_of_segment.test.json)
- [Access to unauthorized memory](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/fail/200/error.unauthorized_memory_access.test.json)
- [Illegal instruction](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/fail/200/error.illegal_instruction.test.json)

### Test Classification

- [Aggregated testing](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/classification/project.test.json)
- [Common configuration settings](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/fail/200/classification/error.out_of_segment.test.json)
  - [Override from project-wide configuration settings](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/classification/merge_config.test.json)
- [Grouping of individual tests](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/classification/group.test.json)

### Misc

- [Customize configurations](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/misc/customize.configurations.test.json)
- [Skip test (ignore)](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/misc/skip.test.json)
- [Complex evaluation](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/misc/evaluation.test.json)
- [Definitions](https://github.com/AsaiYusuke/6502_test_executor/blob/master/example/test/ok/misc/definitions.test.json)

## Dependencies

This project uses the following libraries:

- [Gianluca Ghettini's 6502 emulator](https://github.com/gianlucag/mos6502)
  - [Erik Lothe's 6502 Unit test executor](https://github.com/89erik/6502_test_executor)
- [Taylor C. Richberger's args](https://github.com/Taywee/args)
- [Niels Lohmann's JSON for Modern C++](https://github.com/nlohmann/json)

## License

This project is available under the **MIT license**. See the [LICENSE](LICENSE) file for more information.
