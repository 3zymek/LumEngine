#!/usr/bin/env python3
import os 
import shutil
import zipfile
import urllib.request
from pathlib import Path

def setup_fmod():
    script_dir = Path(__file__).parent
    root_dir = script_dir.parent
    fmod_dir = root_dir / "LumEngine" / "external" / "fmod"
    fmod_zip = fmod_dir / "fmod.zip"
    debug_dir = root_dir / "build" / "Debug"
    release_dir = root_dir / "build" / "Release"

    fmod_dir.mkdir(parents=True, exist_ok=True)

    print("Downloading FMOD...")
    url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.1.0/fmod.zip"
    urllib.request.urlretrieve(url, fmod_zip)

    print("Unpacking FMOD")
    with zipfile.ZipFile(fmod_zip, 'r') as zip_ref:
        zip_ref.extractall(fmod_dir)

    print("Moving FMOD...")
    debug_dir.mkdir(parents=True, exist_ok=True)
    release_dir.mkdir(parents=True, exist_ok=True)

    dll_dir = fmod_dir / "dll"
    if dll_dir.exists():
        for dll_file in dll_dir.glob("*.dll"):
            shutil.copy2(dll_file, debug_dir)
            shutil.copy2(dll_file, release_dir)

    print("Cleaning up...")
    if fmod_zip.exists():
        fmod_zip.unlink()
    if dll_dir.exists():
        shutil.rmtree(dll_dir)
