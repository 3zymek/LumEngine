@echo off
REM =====================================================
REM INIT FMOD - Rozpakowywanie i kopiowanie runtime DLL
REM =====================================================

setlocal

REM -- Ustal root repo (folder nadrzędny względem tego bat)
set SOLUTION_DIR=%~dp0
if "%SOLUTION_DIR:~-1%"=="\" set SOLUTION_DIR=%SOLUTION_DIR:~0,-1%

REM -- Ścieżki
set ZIP_PATH=%SOLUTION_DIR%\LumEngine\engine\external\fmod\fmod_lib.zip
set LIB_DIR=%SOLUTION_DIR%\LumEngine\engine\external\fmod\lib\x64
set TEMP_DIR=%SOLUTION_DIR%\temp_fmod
set EXE_DEBUG=%SOLUTION_DIR%\x64\Debug
set EXE_RELEASE=%SOLUTION_DIR%\x64\Release

echo == INIT FMOD - Rozpakowywanie i kopiowanie runtime DLL ==

REM -- Sprawdź czy ZIP istnieje
if not exist "%ZIP_PATH%" (
    echo ERROR: Nie znaleziono %ZIP_PATH%
    pause
    exit /b 1
)

REM -- Utwórz folder tymczasowy
if exist "%TEMP_DIR%" rmdir /S /Q "%TEMP_DIR%"
mkdir "%TEMP_DIR%"

REM -- Rozpakuj ZIP (PowerShell)
powershell -NoProfile -Command "Expand-Archive -Force '%ZIP_PATH%' '%TEMP_DIR%'"

REM -- Sprawdź czy DLL istnieją
if not exist "%TEMP_DIR%\fmod.dll" (
    echo ERROR: fmod.dll nie znaleziono w archiwum.
    dir "%TEMP_DIR%"
    pause
    exit /b 1
)

REM -- Utwórz foldery exe jeśli trzeba
if not exist "%EXE_DEBUG%" mkdir "%EXE_DEBUG%"
if not exist "%EXE_RELEASE%" mkdir "%EXE_RELEASE%"

REM -- Kopiuj runtime DLL do Debug i Release
xcopy /Y "%TEMP_DIR%\fmod.dll" "%EXE_DEBUG%\" >nul
xcopy /Y "%TEMP_DIR%\fmodstudio.dll" "%EXE_DEBUG%\" >nul
xcopy /Y "%TEMP_DIR%\fmod.dll" "%EXE_RELEASE%\" >nul
xcopy /Y "%TEMP_DIR%\fmodstudio.dll" "%EXE_RELEASE%\" >nul

REM -- Posprzątaj
rmdir /S /Q "%TEMP_DIR%"

echo FMOD runtime skopiowany do:
echo   %EXE_DEBUG%
echo   %EXE_RELEASE%
echo Done.
pause
endlocal
