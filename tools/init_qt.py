#!/usr/bin/env python3
import os
import re
import zipfile
import urllib.request
from pathlib import Path

def create_missing_qt_binaries(qt_root: Path):

    cmake_dir = qt_root / "lib" / "cmake"
    bin_dir = qt_root / "bin"

    if not cmake_dir.exists():
        found_cmake = list(qt_root.glob("**/lib/cmake"))
        if found_cmake:
            cmake_dir = found_cmake[0]
            bin_dir = cmake_dir.parent.parent / "bin"
        else:
            print("Couldn't find path lib/cmake.")
            return

    bin_dir.mkdir(parents=True, exist_ok=True)

    pattern = re.compile(r'bin/([a-zA-Z0-9_\-]+\.exe)', re.IGNORECASE)
    missing_count = 0

    for cmake_file in cmake_dir.rglob("*.cmake"):
        try:
            content = cmake_file.read_text(encoding="utf-8", errors="ignore")
            matches = pattern.findall(content)
            for exe_name in set(matches):
                target_exe = bin_dir / exe_name
                if not target_exe.exists():
                    target_exe.touch()
                    missing_count += 1
                    print(f"Created: {exe_name}")
        except Exception as e:
            print(f"Failed to read {cmake_file}: {e}")

    print(f"Finished. Created Files: {missing_count}")


def setup_qt():
    script_dir = Path(__file__).parent
    root_dir = script_dir.parent
    
    qt_dir = root_dir / "LumEngine" / "External" / "Qt"
    qt_zip = qt_dir / "qt.zip"

    qt_dir.mkdir(parents=True, exist_ok=True)

    print("Downloading Qt...")
    url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.7.0/qt.zip"
    urllib.request.urlretrieve(url, qt_zip)

    print("Unpacking Qt...")
    with zipfile.ZipFile(qt_zip, 'r') as zip_ref:
        zip_ref.extractall(qt_dir)

    print("Cleaning up zip...")
    if qt_zip.exists():
        qt_zip.unlink()

    print("Checking for missing Qt tool binaries...")
    create_missing_qt_binaries(qt_dir)

if __name__ == "__main__":
    setup_qt()