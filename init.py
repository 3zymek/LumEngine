from tools.init_cmake import setup_cmake
from tools.init_fmod import setup_fmod
from tools.init_glfw import setup_glfw

setup_cmake()
setup_fmod()
setup_glfw()

print("Setup finished")
input("Press Enter to exit...")