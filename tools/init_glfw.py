#!/usr/bin/env python3
import os
import zipfile
import shutil
import urllib.request
from pathlib import Path

def setup_glfw():
    scriptDir = Path(__file__).parent
    rootDir = scriptDir.parent
    glfwDir = rootDir / "LumEngine" / "external" / "glfw"
    glfwZip = glfwDir / "glfw.zip"
    debugDir = rootDir / "build" / "Debug"
    releaseDir = rootDir / "build" / "Release"

    glfwDir.mkdir(parents=True, exist_ok=True)

    print("Downloading GLFW...")
    url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.2.0/glfw.zip"
    urllib.request.urlretrieve(url, glfwZip)

    print("Unpacking GLFW...")
    with zipfile.ZipFile(glfwZip, 'r') as zipRef:
        zipRef.extractall(glfwDir)

    print("Moving FMOD...")
    debugDir.mkdir(parents=True, exist_ok=True)
    releaseDir.mkdir(parents=True, exist_ok=True)

    dllDir = glfwDir / "dll"
    if dllDir.exists():
        for dllFile in dllDir.glob("*.dll"):
            shutil.copy2(dllFile, debugDir)
            shutil.copy2(dllFile, releaseDir)

    print("Cleaning up...")
    if glfwDir.exists():
        glfwZip.unlink()
    if dllDir.exists():
        shutil.rmtree(dllDir)