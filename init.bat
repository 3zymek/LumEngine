@echo off

<<<<<<< HEAD
set "initfmod=./init/init_fmod.bat"
set "initcmake=./init/init_cmake.bat"
=======
set "root_dir=%~dp0"
set "initfmod=%root_dir%/init/init_fmod.bat"
set "initcmake=%root_dir%/init/init_cmake.bat"
>>>>>>> 2193e3f (fixed building, vectors3 with C core)

call %initcmake%

cd %root_dir%

call %initfmod%

echo DONE

pause