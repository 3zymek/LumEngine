#!/usr/bin/env python3
import shutil
import urllib.request
import os
import zipfile
from pathlib import Path

def setup_magic_enum():

    scriptDir = Path(__file__).parent
    rootDir = scriptDir.parent

    magicenumDir = rootDir / "LumEngine" / "External" / "MagicEnum"
    magicenumZip = magicenumDir / "magic_enum.zip"

    magicenumDir.mkdir(parents=True, exist_ok=True)

    print("Downloading Magic Enum...")
    url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.13.0/magic_enum.zip"
    urllib.request.urlretrieve(url, magicenumZip)

    print("Unpacking Magic Enum...")
    with zipfile.ZipFile(magicenumZip, 'r') as zipRef:
        zipRef.extractall(magicenumDir)

    print("Cleaning up...")
    if magicenumZip.exists():
        magicenumZip.unlink()

if(__name__ == "__main__"):
    setup_magic_enum()