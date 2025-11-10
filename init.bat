@echo off

set "root_dir=%~dp0"
set "initfmod=%root_dir%/init/init_fmod.bat"
set "initcmake=%root_dir%/init/init_cmake.bat"

call %initcmake%

cd %root_dir%

call %initfmod%

echo DONE

pause