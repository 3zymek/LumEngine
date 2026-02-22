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

    dllDir = fmodDir / "dll"
    if dllDir.exists():
        for dllFile in dllDir.glob("*.dll"):
            shutil.copy2(dllFile, debugDir)
            shutil.copy2(dllFile, releaseDir)

    print("Cleaning up...")
    if fmodZip.exists():
        fmodZip.unlink()
    if dllDir.exists():
        shutil.rmtree(dllDir)
