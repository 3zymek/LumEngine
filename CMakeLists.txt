cmake_minimum_required(VERSION 3.20)
project(LumEngine)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")

# ── LHC ──────────────────────────────────────────────────────────────────────
set(LHC_ROOT_DIR "${CMAKE_SOURCE_DIR}/LumEngine")

find_program(DOTNET_EXECUTABLE dotnet REQUIRED)

add_custom_target(RunLHC ALL
    COMMAND ${DOTNET_EXECUTABLE} "${CMAKE_SOURCE_DIR}/tools/LumenHeaderCompiler.dll" "${LHC_ROOT_DIR}"
    COMMENT "Running LHC..."
    VERBATIM
)

if(MSVC)
    add_compile_options(/Bt)
endif()

# ── ENGINE ────────────────────────────────────────
file(GLOB_RECURSE ENGINE_CPP     "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Src/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/*.hpp"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/*.h"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/*.ipp"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/*.cpp"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/*.c")
file(GLOB_RECURSE EXTERNAL_C     "${CMAKE_SOURCE_DIR}/LumEngine/External/*.c")
file(GLOB_RECURSE EXTERNAL_CPP_IMPL "${CMAKE_SOURCE_DIR}/LumEngine/External/Stb/Src/*.cpp")
file(GLOB_RECURSE ENGINE_C       "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Src/*.c")
file(GLOB_RECURSE ENGINE_IPP     "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Src/*.ipp")
file(GLOB_RECURSE ENGINE_SHADERS "${CMAKE_SOURCE_DIR}/LumEngine/Engine/*.vert"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/Engine/*.frag")

# --- BIBLIOTEKA STATYCZNA (.lib) ---
add_library(LumEngine STATIC
    ${ENGINE_CPP}
    ${ENGINE_HEADERS}
    ${ENGINE_C}
    ${ENGINE_IPP}
    ${ENGINE_SHADERS}
    ${EXTERNAL_C}
    ${EXTERNAL_CPP_IMPL}
)

# === WERSJA Z DLL (ZAKOMENTOWANA NA PRZYSŁOŚĆ) ===
# add_library(LumEngine SHARED
#     ${ENGINE_CPP}
#     ${ENGINE_HEADERS}
#     ${ENGINE_C}
#     ${ENGINE_IPP}
#     ${ENGINE_SHADERS}
#     ${EXTERNAL_C}
#     ${EXTERNAL_CPP_IMPL}
# )
# target_compile_definitions(LumEngine PRIVATE LUM_DYNAMIC_LIBRARIES LUM_ENGINE_BUILD)
# =================================================

add_dependencies(LumEngine RunLHC)
set_source_files_properties(${ENGINE_C}        PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(${EXTERNAL_C}      PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(${EXTERNAL_CPP_IMPL} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

source_group(TREE "${CMAKE_SOURCE_DIR}/LumEngine/Engine" FILES
    ${ENGINE_CPP} ${ENGINE_C} ${ENGINE_HEADERS} ${ENGINE_SHADERS} ${ENGINE_IPP}
)

target_precompile_headers(LumEngine PRIVATE
    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/Core/CorePch.hpp"
    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/Modules/Ahi/AhiPch.hpp"
    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/Modules/Rhi/RhiPch.hpp"
)

target_include_directories(LumEngine PRIVATE
    "${CMAKE_SOURCE_DIR}/LumEngine"
    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include"
    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/Modules"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Glm"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Fmod/Inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Glfw/Inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Assimp/Inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Stb/Inc"
)

target_link_libraries(LumEngine PRIVATE
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Fmod/Lib/fmod_vc.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Glfw/Lib/glfw3.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Glfw/Lib/glfw3_mt.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Glfw/Lib/glfw3dll.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Assimp/Lib/assimp-vc143-mt.lib"
)

# ── EDITOR ────────────────────────────────────────────────────────────────────
file(GLOB_RECURSE EDITOR_CPP     "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Src/*.cpp")
file(GLOB_RECURSE EXTERNAL_IMGUI_SRC "${CMAKE_SOURCE_DIR}/LumEngine/External/Imgui/Src/*.cpp")
file(GLOB_RECURSE EDITOR_HEADERS "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Include/*.hpp"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Include/*.h"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Include/*.cpp"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Include/*.c")

add_executable(LumEngineEditor
    ${EDITOR_CPP}
    ${EDITOR_HEADERS}
    ${EXTERNAL_IMGUI_SRC}
)

target_compile_definitions(LumEngineEditor PRIVATE
    IMGUI_DEFINE_MATH_OPERATORS
)

add_dependencies(LumEngineEditor LumEngine)
source_group(TREE "${CMAKE_SOURCE_DIR}/LumEngine/Editor" FILES
    ${EDITOR_CPP} ${EDITOR_HEADERS}
)

# ── HIDE CONSOLE ──────────────────────────────────────────────
if(WIN32)
    set_target_properties(LumEngineEditor PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    target_link_options(LumEngineEditor PRIVATE
        /ENTRY:mainCRTStartup
    )
endif()

target_include_directories(LumEngineEditor PRIVATE
    "${CMAKE_SOURCE_DIR}/LumEngine"
    "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Include"
    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include"
    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/Modules"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Glm"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Fmod/Inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Glfw/Inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Imgui/Inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Assimp/Inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Stb/Inc"
)

target_precompile_headers(LumEngineEditor PRIVATE
    "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Include/Core/EditorPch.hpp"
)

target_link_libraries(LumEngineEditor PRIVATE
    LumEngine
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Fmod/Lib/fmod_vc.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Glfw/Lib/glfw3.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Glfw/Lib/glfw3_mt.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Glfw/Lib/glfw3dll.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Assimp/Lib/assimp-vc143-mt.lib"
    opengl32.lib
)