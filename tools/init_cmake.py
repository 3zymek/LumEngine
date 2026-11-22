import os
import subprocess
import sys
from pathlib import Path

def setup_cmake():
    
    rootDir = Path(__file__).parent.parent

    buildDir = rootDir / "build"
    debugDir = buildDir / "Debug"
    releaseDir = buildDir / "Release"

    debugDir.mkdir(parents=True, exist_ok=True)
    releaseDir.mkdir(parents=True, exist_ok=True)

    os.chdir(buildDir)

    build_call = [
        "cmake",
        "-B",
        str(buildDir),
        "-S",
        str(rootDir),
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
    ]

    compile_call = [
        "cmake",
        "--build",
        str(buildDir),
        "--target",
        "RunLHC",
        "--config",
        "Release"
    ]

    if sys.platform == "win32":
        compile_call.extend(["--", "/p:SolutionDir=" + str(buildDir)])

    print("Building project...")
    subprocess.check_call(build_call)
    subprocess.check_call(compile_call)