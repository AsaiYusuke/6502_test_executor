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

#define DEFAULT_TIMEOUT 100000


map<uint16_t, uint8_t> data;
map<string, uint16_t> symbol_cache;
vector<string> symbols_file;
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
    //printf("loading cartridge from path: %s\n", file_path);
    streampos size;

    ifstream file (file_path, ios::in|ios::binary|ios::ate);
    if (!file.is_open()) {
        cerr << "Unable to open NES file" << endl;
        throw 1;
    }
    size = file.tellg();
    program = new char[size];
    file.seekg(0, ios::beg);
    file.read(program, size);
    file.close();
}

vector<string> load_file(char* path) {
    streampos size;

    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << path << endl;
        throw 1;
    }
   
    string line;
    vector<string> lines;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    return lines;
}

vector<string> split(string str) {
    string buf;
    stringstream ss(str);
    vector<string> words;
    while (ss >> buf)
    words.push_back(buf);
    return words;
}

uint16_t find_symbol(string label) {
    int offset = 0;
    string::size_type array_offset_start = label.find("[", 0);
    if (array_offset_start != string::npos)
    {
        string::size_type array_offset_end = label.find("]", array_offset_start);
        offset = stoi(label.substr(array_offset_start+1, array_offset_end - array_offset_start-1));
        label = label.substr(0, array_offset_start);
    }

    if (symbol_cache.count(label) == 1) {
        return symbol_cache[label] + offset;
    }
    ostringstream stream;
    stream << "sym\t\"" << label << "\",value=0x";
    string label_pattern = stream.str();
    int label_pattern_size = label_pattern.size();
    for (int i=0; i<symbols_file.size(); i++) {
        string line = symbols_file[i];
        if (line.compare(0, label_pattern_size, label_pattern) == 0) {
            int i;
            for (i=label_pattern_size; line[i] != ','; i++);
            string addr = line.substr(label_pattern_size, i - label_pattern_size);
            symbol_cache[label] = stoi(addr, 0, 16);
            return symbol_cache[label] + offset;
        }
    }
    cerr << "didnt find symbol '" << label << "' in symbols file" << endl;
    throw 2;
}

uint8_t str2byte(string str) {
    int val;

    if (str.compare(0, 2, "0x") == 0)       val = stoi(str.substr(2), 0, 16);
    else if (str.compare(0, 2, "0b") == 0)  val = stoi(str.substr(2), 0, 2);
    else                                    val = stoi(str);

    if (val < 0) val += 256; // convert to two's complement
    
    if (val >> 8) {
        cerr << "value doesn't fit into 8 bits: " << str << endl;
        throw 2;
    }

    return val;
}

void set_pre_conditions(vector<string> test, mos6502 *cpu) {
    for(int i=0; i<test.size(); i++) {
        if (test[i].size() == 0) continue;
        vector<string> words = split(test[i]);
        if ("set" == words[0])
        {
            if ("A" == words[1]) {
                cpu->A = str2byte(words[2]);
            } else if ("X" == words[1]) {
                cpu->X = str2byte(words[2]);
            } else if ("Y" == words[1]) {
                cpu->Y = str2byte(words[2]);
            } else if ("mem" == words[1]) {
                data[find_symbol(words[2])] = str2byte(words[3]);
            } else {
                cerr << "does not understand set " <<  words[1] << endl;
                throw 2;
            }
        }

        if ("start" == words[0]) {
            cpu->pc = find_symbol(words[1]);
        }
    }
}

void assert_equal(int expected, int actual, string message) {
    if (expected != actual) {
        cerr << message << ": Expected " << expected << ", but was " << actual << endl;
        passed = false;
    }
}

void assert_end_condtions(vector<string> test, mos6502 cpu) {
    for(int i=0; i<test.size(); i++) {
        if (test[i].size() == 0) continue;
        vector<string> words = split(test[i]);
        if ("assert" == words[0])
        {
            if ("A" == words[1]) {
                assert_equal(str2byte(words[2]), cpu.A, "Wrong value in A");
            } else if ("X" == words[1]) {
                assert_equal(str2byte(words[2]), cpu.X, "Wrong value in X");
            } else if ("Y" == words[1]) {
                assert_equal(str2byte(words[2]), cpu.Y, "Wrong value in Y");
            } else if ("mem" == words[1]){
                ostringstream msg;
                msg << "Wrong value in " << words[2];
                assert_equal(str2byte(words[3]), data[find_symbol(words[2])], msg.str());
            } else {
                cerr << "does not understand assert " <<  words[1] << endl;
                throw 2;
            }
        }
    }
}

void print_cpu(mos6502* cpu) {
    printf("A=%d, X=%d, Y=%d, pc=%x\n", cpu->A, cpu->X, cpu->Y, cpu->pc);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "usage: " << argv[0] << " <executable> <symbols> <test>" << endl;
        return -1;
    }
    load_nes_cartridge(argv[1]);
    symbols_file = load_file(argv[2]);
    vector<string> test = load_file(argv[3]);

    mos6502 cpu(MemoryRead, MemoryWrite);

    set_pre_conditions(test, &cpu);
    print_cpu(&cpu);

    int count = 0;
    uint16_t pc_start = cpu.pc;
    printf("starting\n");
    do {
        cpu.Run(1);
        printf("pc = %x\n", cpu.pc);
    }
    while (!(cpu.pc > pc_start && cpu.pc <= pc_start + 3) && ++count < DEFAULT_TIMEOUT);

    if (count == DEFAULT_TIMEOUT) {
        cerr << "Stopped after timed out" << endl;
        return -1;
    } else {
        printf("Stopped upon reaching PC=%x\n", cpu.pc);
    }
    print_cpu(&cpu);

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

