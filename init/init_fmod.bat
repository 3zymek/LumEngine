cd /d "%~dp0\.."
set "root_dir=%CD%"

set "fmod_dir=%root_dir%\lumengine\external\fmod"
set "fmod_zip=%fmod_dir%\fmod.zip"
set "debug_dir=%root_dir%\build\Debug"
set "release_dir=%root_dir%\build\Release"

if not exist "%fmod_dir%" mkdir "%fmod_dir%"

echo Downloading FMOD...
PowerShell -Command "Invoke-WebRequest 'https://github.com/3zymek/LumEngineExternal/releases/download/v0.1.0/fmod.zip' -OutFile '%fmod_zip%'"

echo Unpacking FMOD...
PowerShell -Command "Expand-Archive -Path '%fmod_dir%\fmod.zip' -DestinationPath '%fmod_dir%' -Force"

echo Moving FMOD...
if not exist "%debug_dir%" mkdir "%debug_dir%"
if not exist "%release_dir%" mkdir "%release_dir%"
copy /Y "%fmod_dir%\dll\*.dll" "%debug_dir%"
copy /Y "%fmod_dir%\dll\*.dll" "%release_dir%"

echo Cleaning up...
if exist "%fmod_zip%" del "%fmod_zip%"
if exist "%fmod_dir%\dll" rd /S /Q "%fmod_dir%\dll"

echo FMOD DONE