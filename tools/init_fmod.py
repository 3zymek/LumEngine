#!/usr/bin/env python3
import os 
import shutil
import zipfile
import urllib.request
from pathlib import Path

def setup_fmod():
    scriptDir = Path(__file__).parent
    rootDir = scriptDir.parent
    fmodDir = rootDir / "LumEngine" / "external" / "fmod"
    fmodZip = fmodDir / "fmod.zip"
    debugDir = rootDir / "build" / "Debug"
    releaseDir = rootDir / "build" / "Release"

    fmodDir.mkdir(parents=True, exist_ok=True)

    print("Downloading FMOD...")
    url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.1.0/fmod.zip"
    urllib.request.urlretrieve(url, fmodZip)

    print("Unpacking FMOD")
    with zipfile.ZipFile(fmodZip, 'r') as zip_ref:
        zip_ref.extractall(fmodDir)

    print("Moving FMOD...")
    debugDir.mkdir(parents=True, exist_ok=True)
    releaseDir.mkdir(parents=True, exist_ok=True)

    dll_dir = fmodDir / "dll"
    if dll_dir.exists():
        for dll_file in dll_dir.glob("*.dll"):
            shutil.copy2(dll_file, debugDir)
            shutil.copy2(dll_file, releaseDir)

    print("Cleaning up...")
    if fmodZip.exists():
        fmodZip.unlink()
    if dll_dir.exists():
        shutil.rmtree(dll_dir)
