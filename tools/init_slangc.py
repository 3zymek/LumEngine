#!/usr/bin/env python3
import zipfile
import urllib.request
import shutil
from pathlib import Path
import sys

def setup_slangc():

    scriptDir = Path(__file__).parent;
    rootDir = scriptDir.parent;

    slangcDir = rootDir / "LumEngine" / "External" / "Slangc"
    slangcZip = slangcDir / "slangc.zip"
    
    slangcDir.mkdir(parents=True, exist_ok=True)
    
    print("Downloading Slangc...")
    if sys.platform == "win32":
        url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.15.0/slangc.zip"
    elif sys.platform == "darwin":
        url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.14.0/slangc.zip"
    urllib.request.urlretrieve(url, slangcZip)

    print("Unpacking Slangc...")
    with zipfile.ZipFile(slangcZip, 'r') as zipRef:
        zipRef.extractall(slangcDir)

    print("Moving Slangc...")
    if sys.platform == "darwin":

        dllDir = slangcDir / "Dylib"

        dstDir = rootDir / "LumEngine" / "External" / "Dylib"
        dstDir.mkdir(parents=True, exist_ok=True)

        for dllFile in dllDir.glob("*.dylib"):
            shutil.copy2(dllFile, dstDir)

    elif sys.platform == "win32":

        dllDir = slangcDir / "Dll"
        
        debugDir = rootDir / "build" / "Debug"
        releaseDir = rootDir / "build" / "Release"
        
        debugDir.mkdir(parents=True, exist_ok=True)
        releaseDir.mkdir(parents=True, exist_ok=True)
        
        for dllFile in dllDir.glob("*.dll"):
            shutil.copy2(dllFile, debugDir)
            shutil.copy2(dllFile, releaseDir)

    print("Cleaning up...")
    if slangcZip.exists():
        slangcZip.unlink()
    if dllDir.exists():
        shutil.rmtree(dllDir)

if __name__ == "__main__":
    setup_slangc()

