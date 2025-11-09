@echo off

set "initfmod=./init/init_fmod.bat"
set "initcmake=./init/init_cmake.bat"

call %initcmake%

cd %root_dir%

call %initfmod%

echo DONE

pause