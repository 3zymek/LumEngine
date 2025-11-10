set "root_dir=%~dp0"
if "%root_dir:~-1%"=="\" set "root_dir=%root_dir:~0,-1%"
set "real_root_dir=%root_dir%\.."
set "build_dir=%real_root_dir%\build"

echo Building Project...
if not exist %build_dir% mkdir %build_dir%
cd %build_dir%
cmake %real_root_dir%
