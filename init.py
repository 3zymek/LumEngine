from tools.init_cmake import setup_cmake
from tools.init_fmod import setup_fmod
from tools.init_glfw import setup_glfw
from tools.init_assimp import setup_assimp

setup_cmake()
setup_fmod()
setup_glfw()
setup_assimp()

print("Setup finished")
input("Press Enter to exit...")