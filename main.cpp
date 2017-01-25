#include "mos6502.h"


#include <stdio.h>
#include <stdint.h>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define DEFAULT_TIMEOUT 1000


std::map<uint16_t, uint8_t> data;
char* program;
bool passed = true;



uint16_t memory_offset(uint16_t address) {
    return address - 0x8000 + 0x10;
}

uint8_t MemoryRead(uint16_t address) {
    uint8_t val;
    if (address >= 0x8000) {
        val = program[memory_offset(address)];
    } else {
        val = data[address];
    }

    //printf("mem[%x] -> %x\n", address, val);
    return val;
}

void MemoryWrite(uint16_t address, uint8_t value) {
    //printf("mem[%x] <- %x\n", address, value);
    data[address] = value;
}

void load_nes_cartridge(char* file_path) {
    printf("loading cartridge from path: %s\n", file_path);
    streampos size;

    ifstream file (file_path, ios::in|ios::binary|ios::ate);
    
    if (file.is_open()) {
        size = file.tellg();
        program = new char[size];
        file.seekg(0, ios::beg);
        file.read(program, size);
        file.close();
    }
    else cout << "Unable to open file" << endl;
}

vector<string> split(string str) {
    string buf;
    stringstream ss(str);
    vector<string> words;
    while (ss >> buf)
    words.push_back(buf);
    return words;
}

int load_start_address(char* path, string label) {
    ifstream file(path);
    string line;
    ostringstream stream;
    stream << "sym\t\"" << label << "\",value=0x";
    string label_pattern = stream.str();
    int label_pattern_size = label_pattern.size();
    while(getline(file, line)) {
        if (line.compare(0, label_pattern_size, label_pattern) == 0) {
            int i;
            for (i=label_pattern_size; line[i] != ','; i++);
            string addr = line.substr(label_pattern_size, i - label_pattern_size);
            cout << "found the label! addr = " << addr << endl;
            return stoi(addr, 0, 16);
        }
    }
    cout << "didnt find start address in symbols file" << endl;
    throw 2;
}

void set_pre_conditions(vector<string> test, mos6502 *cpu, char* symbols_file) {
    for(int i=0; i<test.size(); i++) {
        vector<string> words = split(test[i]);
        if ("set" == words[0])
        {
            if ("A" == words[1]) {
                cpu->A = stoi(words[2]);
            } else if ("X" == words[1]) {
                cpu->X = stoi(words[2]);
            } else if ("Y" == words[1]) {
                cpu->Y = stoi(words[2]);
            } else if ("mem" == words[1]) {
                data[stoi(words[2])] = stoi(words[3]);
            }
        }

        if ("start" == words[0]) {
            cpu->pc = load_start_address(symbols_file, words[1]);
        }
    }
}

void assert_equal(int expected, int actual, const char message[]) {
    if (expected != actual) {
        printf("%s. Expected %d, but was %d\n", message, expected, actual);
        passed = false;
    }
}

void assert_end_condtions(vector<string> test, mos6502 cpu) {
    for(int i=0; i<test.size(); i++) {
        vector<string> words = split(test[i]);
        if ("assert" == words[0])
        {
            if ("A" == words[1]) {
                assert_equal(stoi(words[2]), cpu.A, "Wrong value in A");
            } else if ("X" == words[1]) {
                assert_equal(stoi(words[2]), cpu.X, "Wrong value in X");
            } else if ("Y" == words[1]) {
                assert_equal(stoi(words[2]), cpu.Y, "Wrong value in Y");
            } else if ("mem" == words[1]){
                assert_equal(stoi(words[3]), data[stoi(words[2])], "Wrong value in memory");
            }
        }
    }
}

vector<string> load_test(char* test_file) {
    printf("loading test from path: %s\n", test_file);
    vector<string> lines;
    string line;
    ifstream file(test_file);
    int i;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    printf("done\n");
    return lines;
}

int main(int argc, char* argv[]) {
    load_nes_cartridge(argv[1]);
    vector<string> test = load_test(argv[3]);

    mos6502 cpu(MemoryRead, MemoryWrite);

    set_pre_conditions(test, &cpu, argv[2]);
    printf("A=%d, X=%d, Y=%d\n", cpu.A, cpu.X, cpu.Y);

    int count = 0;
    uint16_t pc_start = cpu.pc;

    do {
        cpu.Run(1);
    }
    while ((cpu.pc <= pc_start || cpu.pc > pc_start + 3) && count++ < DEFAULT_TIMEOUT);

    if (count == DEFAULT_TIMEOUT) {
        printf("Stopped after timed out\n");
        return -1;
    } else {
        printf("Stopped upon reaching PC=%x\n", cpu.pc);
    }

    printf("Ran %d instructions\n", count);

    assert_end_condtions(test, cpu);
    if (passed) {
        printf("passed! :)\n");
        return 0;
    } else {
        printf("failed! :(\n");
        return 1;
    }
}

