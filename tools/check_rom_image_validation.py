import json
import subprocess
import tempfile
from pathlib import Path


def run_tester(
    executable,
    root,
    image,
    segment_size,
    image_offset=0,
    include_image=True,
    run_test_case=True,
):
    image_path = root / "rom.bin"
    debug_path = root / "test.dbg"
    test_path = root / "test.json"
    if image is not None:
        image_path.write_bytes(image)
    elif image_path.exists():
        image_path.unlink()
    segment_line = (
        f'seg\tid=0,name="CODE",start=0x8000,size=0x{segment_size:04X},type=ro'
    )
    if include_image:
        segment_line += f',oname="{image_path.name}",ooffs={image_offset}'
    debug_path.write_text(
        segment_line
        + '\n'
        + 'sym\tid=0,name="entry",addrsize=absolute,val=0x8000,size=0x0001\n',
        encoding="utf-8",
    )
    test_path.write_text(
        json.dumps(
            {
                "target": {"start": {"label": "entry"}},
                "testCases": (
                    {"ROM image validation": {"setup": {}, "expected": {}}}
                    if run_test_case
                    else {}
                ),
            }
        ),
        encoding="utf-8",
    )
    return subprocess.run(
        [executable, f"--debug={debug_path}", f"--test={test_path}", "--quiet"],
        capture_output=True,
        text=True,
        check=False,
    )


def main():
    executable = str((Path(__file__).resolve().parent.parent / "6502_tester").resolve())

    with tempfile.TemporaryDirectory() as temporary_dir:
        root = Path(temporary_dir)

        for invalid_image in (b"", b"N", b"NES"):
            result = run_tester(executable, root, invalid_image, segment_size=4)
            if result.returncode != 65 or "ROM image is too small" not in result.stderr:
                raise SystemExit(
                    f"Short ROM image was not rejected cleanly: "
                    f"exit={result.returncode}, stderr={result.stderr!r}"
                )

        result = run_tester(
            executable,
            root,
            b"\x60",
            segment_size=1,
            run_test_case=False,
        )
        if result.returncode != 0:
            raise SystemExit(
                f"Valid short non-NES image was not handled safely: "
                f"exit={result.returncode}, stderr={result.stderr!r}"
            )

        for invalid_size, invalid_offset in ((0, 0), (1, -1)):
            result = run_tester(
                executable,
                root,
                b"\x60",
                segment_size=invalid_size,
                image_offset=invalid_offset,
            )
            if result.returncode != 65 or "Invalid ROM range" not in result.stderr:
                raise SystemExit(
                    f"Invalid ROM range was not rejected cleanly: "
                    f"exit={result.returncode}, stderr={result.stderr!r}"
                )

        result = run_tester(executable, root, None, segment_size=1, include_image=False)
        if result.returncode != 65 or "Readonly segment has no ROM image" not in result.stderr:
            raise SystemExit(
                f"Readonly segment without an image was not rejected cleanly: "
                f"exit={result.returncode}, stderr={result.stderr!r}"
            )

        result = run_tester(executable, root, None, segment_size=1)
        if result.returncode != 65 or "Unable to open ROM image" not in result.stderr:
            raise SystemExit(
                f"Missing ROM image was not reported cleanly: "
                f"exit={result.returncode}, stderr={result.stderr!r}"
            )

        large_image = bytearray(65537)
        large_image[:4] = b"NES\x1a"
        large_image[32768] = 0x00
        large_image[65536] = 0x60
        result = run_tester(executable, root, bytes(large_image), segment_size=1, image_offset=65536)
        if result.returncode != 0:
            raise SystemExit(
                f"ROM data beyond 64 KiB was not read correctly: "
                f"exit={result.returncode}, stderr={result.stderr!r}"
            )

        result = run_tester(executable, root, bytes(large_image[:65536]), segment_size=1, image_offset=65536)
        if result.returncode != 65 or "ROM image is too small" not in result.stderr:
            raise SystemExit(
                f"Out-of-file ROM segment was not rejected cleanly: "
                f"exit={result.returncode}, stderr={result.stderr!r}"
            )

    print("Verified ROM bounds, short-image handling, and offsets beyond 64 KiB")


if __name__ == "__main__":
    main()
