#!/usr/bin/env python3
import os 
import shutil
import zipfile
import urllib.request
import sys
from pathlib import Path

def setup_fmod():

    scriptDir = Path(__file__).parent
    rootDir = scriptDir.parent
    
    fmodDir = rootDir / "LumEngine" / "External" / "Fmod"
    fmodZip = fmodDir / "fmod.zip"

    fmodDir.mkdir(parents=True, exist_ok=True)

    print("Downloading FMOD...")
    url = ""
    if sys.platform == "win32":
        url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.1.0/fmod.zip"
    elif sys.platform == "darwin":
        url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.11.0/fmod.zip"
    urllib.request.urlretrieve(url, fmodZip)

    print("Unpacking FMOD")
    with zipfile.ZipFile(fmodZip, 'r') as zip_ref:
        zip_ref.extractall(fmodDir)

    print("Moving FMOD...")
    dllDir = fmodDir / "Dll"
    if dllDir.exists():

        if sys.platform == "darwin":

            macosDllDir = rootDir / "LumEngine" / "External" / "Dylib"

            macosDllDir.mkdir(parents=True, exist_ok=True)
            
            for dllFile in dllDir.glob("*.dylib"):
                shutil.copy2(dllFile, macosDllDir)

        elif sys.platform == "win32":

            debugDir = rootDir / "build" / "Debug"
            releaseDir = rootDir / "build" / "Release"

            debugDir.mkdir(parents=True, exist_ok=True)
            releaseDir.mkdir(parents=True, exist_ok=True)

            for dllFile in dllDir.glob("*.dll"):
                shutil.copy2(dllFile, debugDir)
                shutil.copy2(dllFile, releaseDir)

    print("Cleaning up...")
    if fmodZip.exists():
        fmodZip.unlink()
    if dllDir.exists():
        shutil.rmtree(dllDir)

if __name__ == "__main__":
    setup_fmod()