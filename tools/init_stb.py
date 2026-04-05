#!/usr/bin/env python3
import os
import zipfile
import shutil
import urllib.request
from pathlib import Path

def setup_stb():
    scriptDir = Path(__file__).parent
    rootDir = scriptDir.parent
    stbDir = rootDir / "LumEngine" / "external" / "stb"
    stbZip = stbDir / "stb.zip"

    stbDir.mkdir(parents=True, exist_ok=True)

    print("Downloading STB...")
    url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.6.0/stb.zip"
    urllib.request.urlretrieve(url, stbZip)

    print("Unpacking STB...")
    with zipfile.ZipFile(stbZip, 'r') as zipRef:
        zipRef.extractall(stbDir)

    print("Cleaning up...")
    if stbZip.exists():
        stbZip.unlink()