#!/usr/bin/env python3
import os
import zipfile
import shutil
import urllib.request
from pathlib import Path

def setup_assimp():
    scriptDir = Path(__file__).parent
    rootDir = scriptDir.parent
    
    assimpDir = rootDir / "LumEngine" / "External" / "Assimp"
    assimpZip = assimpDir / "assimp.zip"
    debugDir = rootDir / "build" / "Debug"
    releaseDir = rootDir / "build" / "Release"

    assimpDir.mkdir(parents=True, exist_ok=True)

    print("Downloading Assimp...")
    url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.3.0/assimp.zip"
    urllib.request.urlretrieve(url, assimpZip)

    print("Unpacking Assimp...")
    with zipfile.ZipFile(assimpZip, 'r') as zipRef:
        zipRef.extractall(assimpDir)

    print("Moving Assimp...")
    debugDir.mkdir(parents=True, exist_ok=True)
    releaseDir.mkdir(parents=True, exist_ok=True)

    # Poprawione na "Dll" zgodnie z PascalCase
    dllDir = assimpDir / "Dll"
    if dllDir.exists():
        for dllFile in dllDir.glob("*.dll"):
            shutil.copy2(dllFile, debugDir)
            shutil.copy2(dllFile, releaseDir)

    print("Cleaning up...")
    if assimpZip.exists():
        assimpZip.unlink()
    if dllDir.exists():
        shutil.rmtree(dllDir)