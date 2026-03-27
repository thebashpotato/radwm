import json
import subprocess
import sys
from pathlib import Path
from typing import List, TypedDict


class Target(TypedDict):
    name: str
    sources: List[str]
    packages: List[str]
    ldflags: List[str]


def cmd_output(*args):
    return subprocess.check_output(args, text=True).strip()


def cmd_run(*args):
    subprocess.run(args, check=True)


def pkgconf(flag, package):
    return cmd_output("pkgconf", flag, package)


def gcc(*args):
    return ["gcc", *args]


def clang(*args):
    return ["clang", *args]


def compiler_flags(args: List[str]):
    flags: List[str] = []
    for arg in args:
        f = pkgconf("--cflags", arg)
        if f:
            flags.extend(f.split())
    return flags


def linker_flags(args: List[str]):
    flags: List[str] = []
    for arg in args:
        f = pkgconf("--libs", arg)
        if f:
            flags.extend(f.split())
    return flags


def build_target(
    target: Target,
    build_dir: Path,
    use_gcc: bool,
    warnings: List[str],
    clang_warnings: List[str],
    mode_flags: List[str],
) -> tuple[Path, Path]:
    name = target["name"]
    sources = target["sources"]
    packages = target["packages"]
    extra_ldflags = target["ldflags"]

    cflags = compiler_flags(packages)
    ldflags = linker_flags(packages) + extra_ldflags

    binary = build_dir / name
    mj_file = build_dir / f"{name}.mj"

    if use_gcc:
        cmd_run(
            *clang(
                *clang_warnings,
                *mode_flags,
                *cflags,
                *sources,
                "-fsyntax-only",
                "-MJ",
                str(mj_file),
            )
        )
        cmd_run(
            *gcc(
                *warnings, *mode_flags, *cflags, *sources, "-o", str(binary), *ldflags
            ),
        )
    else:
        cmd_run(
            *clang(
                *warnings,
                *mode_flags,
                *cflags,
                *sources,
                "-o",
                str(binary),
                *ldflags,
                "-MJ",
                str(mj_file),
            )
        )

    return binary, mj_file


def build_targets(
    targets: List[Target],
    test_targets: List[Target],
    build_dir: Path,
    use_gcc: bool,
    warnings: List[str],
    clang_warnings: List[str],
    mode_flags: List[str],
) -> tuple[List[Path], List[Path]]:
    test_binaries: List[Path] = []
    mj_files: List[Path] = []
    for target in targets:
        binary, mj_file = build_target(
            target, build_dir, use_gcc, warnings, clang_warnings, mode_flags
        )
        mj_files.append(mj_file)
        if target in test_targets:
            test_binaries.append(binary)
    return test_binaries, mj_files


def generate_compile_commands(build_dir: Path, mj_files: List[Path]) -> None:
    compile_db = build_dir / "compile_commands.json"
    seen: set[str] = set()
    unique: List[dict] = []
    for mj_file in mj_files:
        content = mj_file.read_text().strip().rstrip(",")
        for entry in json.loads(f"[{content}]"):
            if entry["file"] not in seen:
                seen.add(entry["file"])
                unique.append(entry)
        mj_file.unlink()
    compile_db.write_text(json.dumps(unique, indent=2))


def run_tests(test_binaries: List[Path]) -> None:
    failures = 0
    for binary in test_binaries:
        print(f"Running {binary.name}...")
        result = subprocess.run([str(binary)])
        if result.returncode != 0:
            print(f"FAIL: {binary.name}")
            failures += 1
        else:
            print(f"PASS: {binary.name}")
    if failures:
        print(f"\n{failures} test(s) failed")
        sys.exit(1)
