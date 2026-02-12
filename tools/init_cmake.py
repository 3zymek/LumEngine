import os
import subprocess
from pathlib import Path

def setup_cmake():
    rootDir = Path(__file__).parent.parent

    buildDir = rootDir / "build"
    debugDir = buildDir / "Debug"
    releaseDir = buildDir / "Release"

    debugDir.mkdir(parents=True, exist_ok=True)
    releaseDir.mkdir(parents=True, exist_ok=True)

    os.chdir(buildDir)

    print("Building project...")

    subprocess.check_call(["cmake", rootDir])