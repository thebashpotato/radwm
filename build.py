#!/usr/bin/env python3

import argparse
import shutil
from pathlib import Path
from typing import List

import bs


WARNINGS = [
    "-std=c99",
    "-Wall",
    "-Wextra",
    "-Wpedantic",
    # "-Werror",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-Wstrict-prototypes",
    "-Wold-style-definition",
    "-Wmissing-prototypes",
    "-Wmissing-declarations",
    "-Wredundant-decls",
    "-Wnested-externs",
    "-Wwrite-strings",
    "-Wcast-qual",
    "-Wcast-align",
    "-Wpointer-arith",
    "-Wswitch-enum",
    "-Wunreachable-code",
    "-Wundef",
    "-Wfloat-equal",
    "-Wformat=2",
    "-Wuninitialized",
    "-Wdouble-promotion",
    "-Wnull-dereference",
    "-Walloca",
    "-Wvla",
]

GCC_WARNINGS = [
    "-Winit-self",
    "-Wstrict-overflow=5",
    "-Wjump-misses-init",
    "-Wduplicated-cond",
    "-Wduplicated-branches",
    "-Wlogical-op",
]

CLANG_WARNINGS = [
    "-Wconditional-uninitialized",
    "-Wassign-enum",
    "-Wbad-function-cast",
    "-Wcomma",
    "-Wcovered-switch-default",
    "-Wmissing-variable-declarations",
    "-Wgnu",
]

DEBUG_FLAGS = [
    "-g3",
    "-O0",
    "-fsanitize=leak",
    "-fsanitize=address",
    "-fsanitize=undefined",
    "-fsanitize=float-divide-by-zero",
    "-fsanitize=float-cast-overflow",
    "-fno-omit-frame-pointer",
]

RELEASE_FLAGS = [
    "-O3",
    "-DNDEBUG",
]

TARGETS: List[bs.Target] = [
    {
        "name": "radwm",
        "sources": ["src/geometry.c", "src/client.c", "src/main.c"],
        "packages": ["x11", "xrandr"],
        "ldflags": ["-lm"],
    },
]

TEST_TARGETS: List[bs.Target] = [
    {
        "name": "geometry_tests",
        "sources": ["src/geometry.c", "tests/geometry_tests.c"],
        "packages": [],
        "ldflags": ["-lm"],
    },
]


def main():
    parser = argparse.ArgumentParser(description="Build radwm (The RAD Window Manager)")
    parser.add_argument(
        "--debug", action="store_true", help="build with debug symbols and sanitizers"
    )
    parser.add_argument("--tests", action="store_true", help="build and run unit tests")
    parser.add_argument(
        "--gcc",
        action="store_true",
        help="use gcc instead of clang",
    )
    args = parser.parse_args()

    mode_flags = DEBUG_FLAGS if args.debug else RELEASE_FLAGS
    warnings = WARNINGS + (GCC_WARNINGS if args.gcc else CLANG_WARNINGS)
    clang_warnings = WARNINGS + CLANG_WARNINGS

    build_dir = Path("build")
    if build_dir.exists():
        # rebuild from scratch everytime
        shutil.rmtree(build_dir)

    build_dir.mkdir(exist_ok=True)

    targets = list(TARGETS)
    if args.tests:
        targets.extend(TEST_TARGETS)

    test_binaries, mj_files = bs.build_targets(
        targets, TEST_TARGETS, build_dir, args.gcc, warnings, clang_warnings, mode_flags
    )
    bs.generate_compile_commands(build_dir, mj_files)

    if args.tests and test_binaries:
        bs.run_tests(test_binaries)


if __name__ == "__main__":
    main()
