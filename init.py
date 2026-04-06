from tools.init_cmake import setup_cmake
from tools.init_fmod import setup_fmod
from tools.init_glfw import setup_glfw
from tools.init_assimp import setup_assimp
from tools.init_glm import setup_glm
from tools.init_imgui import setup_imgui
from tools.init_stb import setup_stb


setup_fmod()
setup_imgui()
setup_stb()
setup_glm()
setup_glfw()
setup_assimp()
setup_cmake()

print("Setup finished")
input("Press Enter to exit...")