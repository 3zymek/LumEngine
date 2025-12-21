cd /d "%~dp0\.."
set "root_dir=%CD%"

set "glfw_dir=%root_dir%\lumengine\external\glfw"
set "glfw_zip=%glfw_dir%\glfw.zip"
set "dll_dir=%root_dir%\build\debug"

if not exist "%glfw_dir%" mkdir "%glfw_dir%"

echo Downloading GLFW...
PowerShell -Command "Invoke-WebRequest 'https://github.com/3zymek/LumEngineExternal/releases/download/v0.2.0/glfw.zip' -OutFile '%glfw_zip%'"

echo Unpacking GLFW...
PowerShell -Command "Expand-Archive -Path '%glfw_dir%\glfw.zip' -DestinationPath '%glfw_dir%' -Force"

echo Moving GLFW...
if not exist "%dll_dir%" mkdir "%dll_dir%"
move /Y "%glfw_dir%\*.dll" "%dll_dir%"

echo Cleaning up...
if exist "%glfw_zip%" del "%glfw_zip%"