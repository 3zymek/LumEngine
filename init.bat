@echo off

set "root_dir=%~dp0"
set "initglfw=%root_dir%/init/init_glfw.bat"
set "initfmod=%root_dir%/init/init_fmod.bat"
set "initcmake=%root_dir%/init/init_cmake.bat"

<<<<<<< HEAD
call %initcmake%

cd %root_dir%

=======
cd %root_dir%

call %initcmake%

>>>>>>> dd5bcd8 (reworking events)
call %initfmod%
call %initglfw%

echo DONE

pause