cmake_minimum_required(VERSION 3.20)
project(LumEngine)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")

# ── LHC ──────────────────────────────────────────────────────────────────────
set(LHC_INPUT_DIR    "${CMAKE_SOURCE_DIR}/LumEngine/engine")
set(LHC_SCENE_PARSER "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/modules/scene/format/scene_dependency_manager.hpp")
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

# ── ENGINE (static lib) ───────────────────────────────────────────────────────
file(GLOB_RECURSE ENGINE_CPP     "${CMAKE_SOURCE_DIR}/LumEngine/engine/src/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/*.hpp"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/*.h"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/*.ipp")
file(GLOB_RECURSE EXTERNAL_SRC   "${CMAKE_SOURCE_DIR}/LumEngine/external/imgui/src/*.cpp")
file(GLOB_RECURSE EXTERNAL_C     "${CMAKE_SOURCE_DIR}/LumEngine/external/*.c")
file(GLOB_RECURSE EXTERNAL_CPP_IMPL "${CMAKE_SOURCE_DIR}/LumEngine/external/stb/src/*.cpp")
file(GLOB_RECURSE ENGINE_C       "${CMAKE_SOURCE_DIR}/LumEngine/engine/src/*.c")
file(GLOB_RECURSE ENGINE_IPP     "${CMAKE_SOURCE_DIR}/LumEngine/engine/src/*.ipp")
file(GLOB_RECURSE ENGINE_SHADERS "${CMAKE_SOURCE_DIR}/LumEngine/engine/*.vert"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/engine/*.frag")
file(GLOB_RECURSE EXTERNAL_SRC   "${CMAKE_SOURCE_DIR}/LumEngine/external/imgui/src/*.cpp")

add_library(LumEngine STATIC
    ${ENGINE_CPP}
    ${ENGINE_HEADERS}
    ${ENGINE_C}
    ${ENGINE_IPP}
    ${ENGINE_SHADERS}
    ${EXTERNAL_SRC}
    ${EXTERNAL_C}
    ${EXTERNAL_CPP_IMPL}
)
add_dependencies(LumEngine RunLHC)
set_source_files_properties(${ENGINE_C}    PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(${EXTERNAL_C}  PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(${EXTERNAL_CPP_IMPL} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

source_group(TREE "${CMAKE_SOURCE_DIR}/LumEngine/engine" FILES
    ${ENGINE_CPP} ${ENGINE_C} ${ENGINE_HEADERS} ${ENGINE_SHADERS} ${ENGINE_IPP}
)

target_precompile_headers(LumEngine PRIVATE
    "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/core/core_pch.hpp"
    "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/modules/ahi/ahi_pch.hpp"
    "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/modules/rhi/rhi_pch.hpp"
)

target_include_directories(LumEngine PRIVATE
    "${CMAKE_SOURCE_DIR}/LumEngine/engine/include"
    "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/modules"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/glm"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/fmod/inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/glfw/inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/imgui/inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/assimp/inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/stb/inc"
)

target_link_libraries(LumEngine PRIVATE
    "${CMAKE_SOURCE_DIR}/LumEngine/external/fmod/lib/fmod_vc.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/glfw/lib/glfw3.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/glfw/lib/glfw3_mt.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/glfw/lib/glfw3dll.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/assimp/lib/assimp-vc143-mt.lib"
)

# ── EDITOR (executable) ───────────────────────────────────────────────────────
file(GLOB_RECURSE EDITOR_CPP     "${CMAKE_SOURCE_DIR}/LumEngine/editor/src/*.cpp")
file(GLOB_RECURSE EDITOR_HEADERS "${CMAKE_SOURCE_DIR}/LumEngine/editor/include/*.hpp"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/editor/include/*.h")

add_executable(LumEngineEditor
    ${EDITOR_CPP}
    ${EDITOR_HEADERS}
)
add_dependencies(LumEngineEditor LumEngine)

source_group(TREE "${CMAKE_SOURCE_DIR}/LumEngine/editor" FILES
    ${EDITOR_CPP} ${EDITOR_HEADERS}
)

target_include_directories(LumEngineEditor PRIVATE
    "${CMAKE_SOURCE_DIR}/LumEngine/editor/include"
    "${CMAKE_SOURCE_DIR}/LumEngine/engine/include"
    "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/modules"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/glm"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/fmod/inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/glfw/inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/imgui/inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/assimp/inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/stb/inc"
)

target_link_libraries(LumEngineEditor PRIVATE 
    LumEngine
    "${CMAKE_SOURCE_DIR}/LumEngine/external/glfw/lib/glfw3.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/glfw/lib/glfw3_mt.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/glfw/lib/glfw3dll.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/assimp/lib/assimp-vc143-mt.lib"
    opengl32.lib
)