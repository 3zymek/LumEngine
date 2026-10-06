#!/usr/bin/env python3
import os
import sys
import urllib.request
import zipfile
import shutil
from pathlib import Path

def setup_glfw():

    root_dir = Path(__file__).parent.parent.resolve()
    glfw_dir = root_dir / "LumEngine" / "external" / "glfw"
    glfw_zip = glfw_dir / "glfw.zip"

    build_dir = root_dir / "build" / "debug"

    glfw_dir.mkdir(parents=True, exist_ok=True)

    print("Downloading GLFW...")
    url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.2.0/glfw.zip"
    try:
        urllib.request.urlretrieve(url, glfw_zip)
    except Exception as e:
        print(f"Error downloading GLFW: {e}", file=sys.stderr)

    print("Unpacking GLFW...")
    try:
        with zipfile.ZipFile(glfw_zip, 'r') as zip_ref:
            zip_ref.extractall(glfw_dir)
    except Exception as e:
        print(f"Error unpacking GLFW: {e}", file=sys.stderr)

    print("Moving GLFW libraries...")
    build_dir.mkdir(parents=True, exist_ok=True)

    if sys.platform == "win32":
        extensions = ["*.dll"]
    elif sys.platform == "darwin":
        extensions = ["*.dylib"]
    else:
        extensions = ["*.so", "*.so.*"]

    moved_files = False
    for ext in extensions:
        for file in glfw_dir.glob(ext):
            dest = build_dir / file.name
            shutil.move(str(file), str(dest))
            print(f"Moved: {file.name}")
            moved_files = True

    if not moved_files:
        print("WARNING: No library files found to move")

    print("Cleaning up...")
    if glfw_zip.exists():
        glfw_zip.unlink()