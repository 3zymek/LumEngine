#!/usr/bin/env python3
import os
import zipfile
import shutil
import urllib.request
from pathlib import Path

def setup_glm():
    scriptDir = Path(__file__).parent
    rootDir = scriptDir.parent
    glmDir = rootDir / "LumEngine" / "external" / "glm"
    glmZip = glmDir / "glm.zip"

    glmDir.mkdir(parents=True, exist_ok=True)

    print("Downloading GLM...")
    url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.4.0/glm.zip"
    urllib.request.urlretrieve(url, glmZip)

    print("Unpacking GLM...")
    with zipfile.ZipFile(glmZip, 'r') as zipRef:
        zipRef.extractall(glmDir)

    print("Cleaning up...")
    if glmZip.exists():
        glmZip.unlink()