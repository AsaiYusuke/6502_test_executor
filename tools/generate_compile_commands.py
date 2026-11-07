#!/usr/bin/env python3

import json
import shlex
import sys
from pathlib import Path


def main() -> None:
    root = Path(sys.argv[1]).resolve()
    build_dir = Path(sys.argv[2])
    compiler = shlex.split(sys.argv[3])
    flags = shlex.split(sys.argv[4])
    sources = shlex.split(sys.argv[5])
    extra_source = Path(sys.argv[6])
    extra_output = Path(sys.argv[7])

    entries = []
    for source_name in sources:
        source = Path(source_name)
        output = build_dir / source.relative_to("src").with_suffix(".o")
        entries.append((source, output))
    entries.append((extra_source, extra_output))

    database = [
        {
            "directory": str(root),
            "file": str(source),
            "arguments": [
                *compiler,
                *flags,
                "-c",
                "-o",
                str(output),
                str(source),
            ],
            "output": str(output),
        }
        for source, output in entries
    ]

    destination = root / "compile_commands.json"
    destination.write_text(json.dumps(database, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
