#!/usr/bin/env python3
import os
import zipfile
import shutil
import urllib.request
from pathlib import Path

def setup_assimp():
    scriptDir = Path(__file__).parent
    rootDir = scriptDir.parent
    assimpDir = rootDir / "LumEngine" / "external" / "assimp"
    assimpZip = assimpDir / "assimp.zip"
    
