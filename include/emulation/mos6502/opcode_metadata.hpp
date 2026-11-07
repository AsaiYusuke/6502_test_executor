#pragma once

#include <cstdint>

namespace mos6502_metadata
{
enum class operation
{
    ADC, AND, ASL, ASL_ACC, BCC, BCS, BEQ, BIT, BMI, BNE, BPL, BRK, BVC, BVS,
    CLC, CLD, CLI, CLV, CMP, CPX, CPY, DEC, DEX, DEY, EOR, INC, INX, INY, JMP,
    JSR, LDA, LDX, LDY, LSR, LSR_ACC, NOP, ORA, PHA, PHP, PLA, PLP, ROL,
    ROL_ACC, ROR, ROR_ACC, RTI, RTS, SBC, SEC, SED, SEI, STA, STX, STY, TAX,
    TAY, TSX, TXA, TXS, TYA, invalid
};

enum class addressing_mode
{
    ABI, ABS, ABX, ABY, ACC, IMM, IMP, INX, INY, REL, ZER, ZEX, ZEY
};

struct instruction
{
    operation op;
    addressing_mode mode;
};

constexpr instruction decode(uint8_t opcode)
{
    

    switch (opcode)
    {
    case 0x69: return {operation::ADC, addressing_mode::IMM};
    case 0x65: return {operation::ADC, addressing_mode::ZER};
    case 0x75: return {operation::ADC, addressing_mode::ZEX};
    case 0x6D: return {operation::ADC, addressing_mode::ABS};
    case 0x7D: return {operation::ADC, addressing_mode::ABX};
    case 0x79: return {operation::ADC, addressing_mode::ABY};
    case 0x61: return {operation::ADC, addressing_mode::INX};
    case 0x71: return {operation::ADC, addressing_mode::INY};
    case 0x29: return {operation::AND, addressing_mode::IMM};
    case 0x25: return {operation::AND, addressing_mode::ZER};
    case 0x35: return {operation::AND, addressing_mode::ZEX};
    case 0x2D: return {operation::AND, addressing_mode::ABS};
    case 0x3D: return {operation::AND, addressing_mode::ABX};
    case 0x39: return {operation::AND, addressing_mode::ABY};
    case 0x21: return {operation::AND, addressing_mode::INX};
    case 0x31: return {operation::AND, addressing_mode::INY};
    case 0x0A: return {operation::ASL_ACC, addressing_mode::ACC};
    case 0x06: return {operation::ASL, addressing_mode::ZER};
    case 0x16: return {operation::ASL, addressing_mode::ZEX};
    case 0x0E: return {operation::ASL, addressing_mode::ABS};
    case 0x1E: return {operation::ASL, addressing_mode::ABX};
    case 0x90: return {operation::BCC, addressing_mode::REL};
    case 0xB0: return {operation::BCS, addressing_mode::REL};
    case 0xF0: return {operation::BEQ, addressing_mode::REL};
    case 0x24: return {operation::BIT, addressing_mode::ZER};
    case 0x2C: return {operation::BIT, addressing_mode::ABS};
    case 0x30: return {operation::BMI, addressing_mode::REL};
    case 0xD0: return {operation::BNE, addressing_mode::REL};
    case 0x10: return {operation::BPL, addressing_mode::REL};
    case 0x00: return {operation::BRK, addressing_mode::IMP};
    case 0x50: return {operation::BVC, addressing_mode::REL};
    case 0x70: return {operation::BVS, addressing_mode::REL};
    case 0x18: return {operation::CLC, addressing_mode::IMP};
    case 0xD8: return {operation::CLD, addressing_mode::IMP};
    case 0x58: return {operation::CLI, addressing_mode::IMP};
    case 0xB8: return {operation::CLV, addressing_mode::IMP};
    case 0xC9: return {operation::CMP, addressing_mode::IMM};
    case 0xC5: return {operation::CMP, addressing_mode::ZER};
    case 0xD5: return {operation::CMP, addressing_mode::ZEX};
    case 0xCD: return {operation::CMP, addressing_mode::ABS};
    case 0xDD: return {operation::CMP, addressing_mode::ABX};
    case 0xD9: return {operation::CMP, addressing_mode::ABY};
    case 0xC1: return {operation::CMP, addressing_mode::INX};
    case 0xD1: return {operation::CMP, addressing_mode::INY};
    case 0xE0: return {operation::CPX, addressing_mode::IMM};
    case 0xE4: return {operation::CPX, addressing_mode::ZER};
    case 0xEC: return {operation::CPX, addressing_mode::ABS};
    case 0xC0: return {operation::CPY, addressing_mode::IMM};
    case 0xC4: return {operation::CPY, addressing_mode::ZER};
    case 0xCC: return {operation::CPY, addressing_mode::ABS};
    case 0xC6: return {operation::DEC, addressing_mode::ZER};
    case 0xD6: return {operation::DEC, addressing_mode::ZEX};
    case 0xCE: return {operation::DEC, addressing_mode::ABS};
    case 0xDE: return {operation::DEC, addressing_mode::ABX};
    case 0xCA: return {operation::DEX, addressing_mode::IMP};
    case 0x88: return {operation::DEY, addressing_mode::IMP};
    case 0x49: return {operation::EOR, addressing_mode::IMM};
    case 0x45: return {operation::EOR, addressing_mode::ZER};
    case 0x55: return {operation::EOR, addressing_mode::ZEX};
    case 0x4D: return {operation::EOR, addressing_mode::ABS};
    case 0x5D: return {operation::EOR, addressing_mode::ABX};
    case 0x59: return {operation::EOR, addressing_mode::ABY};
    case 0x41: return {operation::EOR, addressing_mode::INX};
    case 0x51: return {operation::EOR, addressing_mode::INY};
    case 0xE6: return {operation::INC, addressing_mode::ZER};
    case 0xF6: return {operation::INC, addressing_mode::ZEX};
    case 0xEE: return {operation::INC, addressing_mode::ABS};
    case 0xFE: return {operation::INC, addressing_mode::ABX};
    case 0xE8: return {operation::INX, addressing_mode::IMP};
    case 0xC8: return {operation::INY, addressing_mode::IMP};
    case 0x4C: return {operation::JMP, addressing_mode::ABS};
    case 0x6C: return {operation::JMP, addressing_mode::ABI};
    case 0x20: return {operation::JSR, addressing_mode::ABS};
    case 0xA9: return {operation::LDA, addressing_mode::IMM};
    case 0xA5: return {operation::LDA, addressing_mode::ZER};
    case 0xB5: return {operation::LDA, addressing_mode::ZEX};
    case 0xAD: return {operation::LDA, addressing_mode::ABS};
    case 0xBD: return {operation::LDA, addressing_mode::ABX};
    case 0xB9: return {operation::LDA, addressing_mode::ABY};
    case 0xA1: return {operation::LDA, addressing_mode::INX};
    case 0xB1: return {operation::LDA, addressing_mode::INY};
    case 0xA2: return {operation::LDX, addressing_mode::IMM};
    case 0xA6: return {operation::LDX, addressing_mode::ZER};
    case 0xB6: return {operation::LDX, addressing_mode::ZEY};
    case 0xAE: return {operation::LDX, addressing_mode::ABS};
    case 0xBE: return {operation::LDX, addressing_mode::ABY};
    case 0xA0: return {operation::LDY, addressing_mode::IMM};
    case 0xA4: return {operation::LDY, addressing_mode::ZER};
    case 0xB4: return {operation::LDY, addressing_mode::ZEX};
    case 0xAC: return {operation::LDY, addressing_mode::ABS};
    case 0xBC: return {operation::LDY, addressing_mode::ABX};
    case 0x4A: return {operation::LSR_ACC, addressing_mode::ACC};
    case 0x46: return {operation::LSR, addressing_mode::ZER};
    case 0x56: return {operation::LSR, addressing_mode::ZEX};
    case 0x4E: return {operation::LSR, addressing_mode::ABS};
    case 0x5E: return {operation::LSR, addressing_mode::ABX};
    case 0xEA: return {operation::NOP, addressing_mode::IMP};
    case 0x09: return {operation::ORA, addressing_mode::IMM};
    case 0x05: return {operation::ORA, addressing_mode::ZER};
    case 0x15: return {operation::ORA, addressing_mode::ZEX};
    case 0x0D: return {operation::ORA, addressing_mode::ABS};
    case 0x1D: return {operation::ORA, addressing_mode::ABX};
    case 0x19: return {operation::ORA, addressing_mode::ABY};
    case 0x01: return {operation::ORA, addressing_mode::INX};
    case 0x11: return {operation::ORA, addressing_mode::INY};
    case 0x48: return {operation::PHA, addressing_mode::IMP};
    case 0x08: return {operation::PHP, addressing_mode::IMP};
    case 0x68: return {operation::PLA, addressing_mode::IMP};
    case 0x28: return {operation::PLP, addressing_mode::IMP};
    case 0x2A: return {operation::ROL_ACC, addressing_mode::ACC};
    case 0x26: return {operation::ROL, addressing_mode::ZER};
    case 0x36: return {operation::ROL, addressing_mode::ZEX};
    case 0x2E: return {operation::ROL, addressing_mode::ABS};
    case 0x3E: return {operation::ROL, addressing_mode::ABX};
    case 0x6A: return {operation::ROR_ACC, addressing_mode::ACC};
    case 0x66: return {operation::ROR, addressing_mode::ZER};
    case 0x76: return {operation::ROR, addressing_mode::ZEX};
    case 0x6E: return {operation::ROR, addressing_mode::ABS};
    case 0x7E: return {operation::ROR, addressing_mode::ABX};
    case 0x40: return {operation::RTI, addressing_mode::IMP};
    case 0x60: return {operation::RTS, addressing_mode::IMP};
    case 0xE9: return {operation::SBC, addressing_mode::IMM};
    case 0xE5: return {operation::SBC, addressing_mode::ZER};
    case 0xF5: return {operation::SBC, addressing_mode::ZEX};
    case 0xED: return {operation::SBC, addressing_mode::ABS};
    case 0xFD: return {operation::SBC, addressing_mode::ABX};
    case 0xF9: return {operation::SBC, addressing_mode::ABY};
    case 0xE1: return {operation::SBC, addressing_mode::INX};
    case 0xF1: return {operation::SBC, addressing_mode::INY};
    case 0x38: return {operation::SEC, addressing_mode::IMP};
    case 0xF8: return {operation::SED, addressing_mode::IMP};
    case 0x78: return {operation::SEI, addressing_mode::IMP};
    case 0x85: return {operation::STA, addressing_mode::ZER};
    case 0x95: return {operation::STA, addressing_mode::ZEX};
    case 0x8D: return {operation::STA, addressing_mode::ABS};
    case 0x9D: return {operation::STA, addressing_mode::ABX};
    case 0x99: return {operation::STA, addressing_mode::ABY};
    case 0x81: return {operation::STA, addressing_mode::INX};
    case 0x91: return {operation::STA, addressing_mode::INY};
    case 0x86: return {operation::STX, addressing_mode::ZER};
    case 0x96: return {operation::STX, addressing_mode::ZEY};
    case 0x8E: return {operation::STX, addressing_mode::ABS};
    case 0x84: return {operation::STY, addressing_mode::ZER};
    case 0x94: return {operation::STY, addressing_mode::ZEX};
    case 0x8C: return {operation::STY, addressing_mode::ABS};
    case 0xAA: return {operation::TAX, addressing_mode::IMP};
    case 0xA8: return {operation::TAY, addressing_mode::IMP};
    case 0xBA: return {operation::TSX, addressing_mode::IMP};
    case 0x8A: return {operation::TXA, addressing_mode::IMP};
    case 0x9A: return {operation::TXS, addressing_mode::IMP};
    case 0x98: return {operation::TYA, addressing_mode::IMP};
    default: return {operation::invalid, addressing_mode::IMP};
    }
}
}
