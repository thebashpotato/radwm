import json
import subprocess
import sys
from pathlib import Path
from typing import List, TypedDict


class Library(TypedDict):
    name: str
    sources: List[str]
    include_dirs: List[str]


class Target(TypedDict):
    name: str
    sources: List[str]
    packages: List[str]
    ldflags: List[str]
    libs: List[str]


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


def build_library(
    lib: Library,
    build_dir: Path,
    use_gcc: bool,
    warnings: List[str],
    clang_warnings: List[str],
    mode_flags: List[str],
) -> tuple[Path, List[str], List[Path]]:
    name = lib["name"]
    sources = lib["sources"]
    include_dirs = lib["include_dirs"]

    include_flags = [f"-I{d}" for d in include_dirs]
    obj_dir = build_dir / name
    obj_dir.mkdir(exist_ok=True)

    obj_files: List[str] = []
    mj_files: List[Path] = []

    for src in sources:
        stem = Path(src).stem
        obj = obj_dir / f"{stem}.o"
        mj_file = obj_dir / f"{stem}.mj"
        obj_files.append(str(obj))
        mj_files.append(mj_file)

        if use_gcc:
            cmd_run(
                *clang(
                    *clang_warnings,
                    *mode_flags,
                    *include_flags,
                    src,
                    "-fsyntax-only",
                    "-MJ",
                    str(mj_file),
                )
            )
            cmd_run(
                *gcc(
                    *warnings, *mode_flags, *include_flags, "-c", src, "-o", str(obj)
                ),
            )
        else:
            cmd_run(
                *clang(
                    *warnings,
                    *mode_flags,
                    *include_flags,
                    "-c",
                    src,
                    "-o",
                    str(obj),
                    "-MJ",
                    str(mj_file),
                )
            )

    archive = build_dir / f"lib{name}.a"
    cmd_run("ar", "rcs", str(archive), *obj_files)

    return archive, include_flags, mj_files


def build_libraries(
    libraries: List[Library],
    build_dir: Path,
    use_gcc: bool,
    warnings: List[str],
    clang_warnings: List[str],
    mode_flags: List[str],
) -> tuple[dict[str, tuple[Path, List[str]]], List[Path]]:
    built_libs: dict[str, tuple[Path, List[str]]] = {}
    all_mj_files: List[Path] = []

    for lib in libraries:
        archive, include_flags, mj_files = build_library(
            lib, build_dir, use_gcc, warnings, clang_warnings, mode_flags
        )
        built_libs[lib["name"]] = (archive, include_flags)
        all_mj_files.extend(mj_files)

    return built_libs, all_mj_files


def build_target(
    target: Target,
    build_dir: Path,
    use_gcc: bool,
    warnings: List[str],
    clang_warnings: List[str],
    mode_flags: List[str],
    built_libs: dict[str, tuple[Path, List[str]]],
) -> tuple[Path, Path]:
    name = target["name"]
    sources = target["sources"]
    packages = target["packages"]
    extra_ldflags = target["ldflags"]
    libs = target["libs"]

    cflags = compiler_flags(packages)
    ldflags = linker_flags(packages) + extra_ldflags

    lib_include_flags: List[str] = []
    lib_archives: List[str] = []
    for lib_name in libs:
        archive, include_flags = built_libs[lib_name]
        lib_include_flags.extend(include_flags)
        lib_archives.append(str(archive))

    binary = build_dir / name
    mj_file = build_dir / f"{name}.mj"

    if use_gcc:
        cmd_run(
            *clang(
                *clang_warnings,
                *mode_flags,
                *cflags,
                *lib_include_flags,
                *sources,
                "-fsyntax-only",
                "-MJ",
                str(mj_file),
            )
        )
        cmd_run(
            *gcc(
                *warnings,
                *mode_flags,
                *cflags,
                *lib_include_flags,
                *sources,
                "-o",
                str(binary),
                *lib_archives,
                *ldflags,
            ),
        )
    else:
        cmd_run(
            *clang(
                *warnings,
                *mode_flags,
                *cflags,
                *lib_include_flags,
                *sources,
                "-o",
                str(binary),
                *lib_archives,
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
    built_libs: dict[str, tuple[Path, List[str]]],
) -> tuple[List[Path], List[Path]]:
    test_binaries: List[Path] = []
    mj_files: List[Path] = []
    for target in targets:
        binary, mj_file = build_target(
            target, build_dir, use_gcc, warnings, clang_warnings, mode_flags, built_libs
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
