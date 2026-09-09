#!/usr/bin/env python3
import zipfile
import urllib.request
from pathlib import Path

def setup_vulkan():
    scriptDir = Path(__file__).parent
    engineRootDir = scriptDir.parent

    vulkanDir = engineRootDir / "LumEngine" / "External" / "Vulkan"
    vulkanZip = vulkanDir / "vulkan.zip"

    vulkanDir.mkdir(parents=True, exist_ok=True)

    print("Downloading Vulkan SDK...")
    url = "https://github.com/3zymek/LumEngineExternal/releases/download/v0.8.0/vulkan.zip"
    urllib.request.urlretrieve(url, vulkanZip)

    print("Extracting Vulkan SDK...")
    
    with zipfile.ZipFile(vulkanZip, 'r') as zip_ref:
        zip_ref.extractall(vulkanDir)
    
    print("Cleaning up...")
    
    if vulkanZip.exists(): 
        vulkanZip.unlink()