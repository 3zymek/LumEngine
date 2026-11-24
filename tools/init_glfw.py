#!/usr/bin/env python3
import sys
import zipfile
import shutil
import urllib.request
from pathlib import Path

def setup_glfw():

    scriptDir = Path(__file__).parent
    rootDir = scriptDir.parent
    
    glfwDir = rootDir / "LumEngine" / "External" / "Glfw"
    glfwZip = glfwDir / "glfw.zip"

    glfwDir.mkdir(parents=True, exist_ok=True)

    print("Downloading GLFW...")
    url = ""
    if sys.platform == "win32":
        url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.2.0/glfw.zip"
    elif sys.platform == "darwin":
        url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.9.0/glfw.zip"
    urllib.request.urlretrieve(url, glfwZip)

    print("Unpacking GLFW...")
    with zipfile.ZipFile(glfwZip, 'r') as zipRef:
        zipRef.extractall(glfwDir)

    print("Moving GLFW...")
    if sys.platform == "darwin":

        dllDir = glfwDir / "Dylib"

        dstDir = rootDir / "LumEngine" / "External" / "Dylib"
        dstDir.mkdir(parents=True, exist_ok=True)

        for dllFile in dllDir.glob("*.dylib"):
            shutil.copy2(dllFile, dstDir)

    elif sys.platform == "win32":

        dllDir = glfwDir / "Dll"
        
        debugDir = rootDir / "build" / "Debug"
        releaseDir = rootDir / "build" / "Release"
        
        debugDir.mkdir(parents=True, exist_ok=True)
        releaseDir.mkdir(parents=True, exist_ok=True)
        
        for dllFile in dllDir.glob("*.dll"):
            shutil.copy2(dllFile, debugDir)
            shutil.copy2(dllFile, releaseDir)

    print("Cleaning up...")
    if glfwZip.exists():
        glfwZip.unlink()
    if dllDir.exists():
        shutil.rmtree(dllDir)

if __name__ == "__main__":
    setup_glfw()