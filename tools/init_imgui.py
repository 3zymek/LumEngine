#!/usr/bin/env python3
import os
import zipfile
import shutil
import urllib.request
from pathlib import Path

def setup_imgui():
    scriptDir = Path(__file__).parent
    rootDir = scriptDir.parent
    imguiDir = rootDir / "LumEngine" / "external" / "imgui"
    imguiZip = imguiDir / "imgui.zip"

    imguiDir.mkdir(parents=True, exist_ok=True)

    print("Downloading ImGui...")
    url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.5.0/imgui.zip"
    urllib.request.urlretrieve(url, imguiZip)

    print("Unpacking ImGui...")
    with zipfile.ZipFile(imguiZip, 'r') as zipRef:
        zipRef.extractall(imguiDir)

    print("Cleaning up...")
    if imguiZip.exists():
        imguiZip.unlink()