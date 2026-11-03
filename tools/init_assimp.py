#!/usr/bin/env python3
import os
import sys
import zipfile
import shutil
import urllib.request
from pathlib import Path

def setup_assimp():

    scriptDir = Path(__file__).parent
    rootDir = scriptDir.parent
    
    assimpDir = rootDir / "LumEngine" / "External" / "Assimp"
    assimpZip = assimpDir / "assimp.zip"

    assimpDir.mkdir(parents=True, exist_ok=True)

    print("Downloading Assimp...")
    url = ""
    if sys.platform == "win32":
        url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.3.0/assimp.zip"
    elif sys.platform == "darwin":
        url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.12.0/assimp.zip"
    urllib.request.urlretrieve(url, assimpZip)

    print("Unpacking Assimp...")
    with zipfile.ZipFile(assimpZip, 'r') as zipRef:
        zipRef.extractall(assimpDir)

    print("Moving Assimp...")
    dllDir = assimpDir / "Dll"
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
    if assimpZip.exists():
        assimpZip.unlink()
    if dllDir.exists():
        shutil.rmtree(dllDir)