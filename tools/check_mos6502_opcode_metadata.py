import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
MOS6502_SOURCE = ROOT / "third_party" / "mos6502" / "mos6502.cpp"
METADATA_HEADER = ROOT / "include" / "emulation" / "mos6502" / "opcode_metadata.hpp"
INSTRUCTION_PATTERN = re.compile(
    r"MAKE_INSTR\((0x[0-9A-Fa-f]+),\s*(\w+),\s*(\w+),"
)
METADATA_PATTERN = re.compile(
    r"case (0x[0-9A-Fa-f]+): return "
    r"\{operation::(\w+), addressing_mode::(\w+)\};"
)


def main():
    source_instructions = {}
    skipped_illegal_block_depth = 0

    for line in MOS6502_SOURCE.read_text(encoding="utf-8").splitlines():
        directive = line.strip()
        if skipped_illegal_block_depth:
            if re.match(r"#\s*(if|ifdef|ifndef)\b", directive):
                skipped_illegal_block_depth += 1
            elif re.match(r"#\s*endif\b", directive):
                skipped_illegal_block_depth -= 1
            continue

        if directive == "#ifdef ILLEGAL_OPCODES":
            skipped_illegal_block_depth = 1
            continue

        match = INSTRUCTION_PATTERN.search(line)
        if match:
            opcode, mnemonic, mode = match.groups()
            source_instructions[int(opcode, 16)] = (mnemonic, mode)

    metadata = {
        int(opcode, 16): (mnemonic, mode)
        for opcode, mnemonic, mode in METADATA_PATTERN.findall(
            METADATA_HEADER.read_text(encoding="utf-8")
        )
    }

    if source_instructions != metadata:
        missing = sorted(source_instructions.keys() - metadata.keys())
        extra = sorted(metadata.keys() - source_instructions.keys())
        mismatched = sorted(
            (opcode, source_instructions[opcode], metadata[opcode])
            for opcode in source_instructions.keys() & metadata.keys()
            if source_instructions[opcode] != metadata[opcode]
        )
        raise SystemExit(
            f"MOS 6502 opcode metadata mismatch: "
            f"missing={missing}, extra={extra}, mismatched={mismatched}"
        )

    print(f"Verified {len(metadata)} MOS 6502 opcodes against upstream source")


if __name__ == "__main__":
    main()
