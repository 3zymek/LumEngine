cmake_minimum_required(VERSION 3.20)
project(LumEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
set(LHC_EXE "${CMAKE_SOURCE_DIR}/tools/LHC.exe")
set(LHC_INPUT_DIR "${CMAKE_SOURCE_DIR}/LumEngine/engine")
set(LHC_SCENE_PARSER "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/modules/scene/format/scene_dependency_manager.hpp")
set(LHC_OUTPUT "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/modules/scene/format/scene_dependency_manager.generated.hpp")

find_program(DOTNET_EXECUTABLE dotnet REQUIRED)

add_custom_command(
    OUTPUT "${LHC_OUTPUT}"
    COMMAND ${DOTNET_EXECUTABLE} "${CMAKE_SOURCE_DIR}/tools/LumenHeaderCompiler.dll" "${LHC_INPUT_DIR}" "${LHC_SCENE_PARSER}"
    COMMENT "Running LHC..."
    VERBATIM
)

add_custom_target(RunLHC
    DEPENDS "${LHC_OUTPUT}"
)

if(MSVC)
    add_compile_options(/Bt)
endif()

file(GLOB_RECURSE ENGINE_CPP     "${CMAKE_SOURCE_DIR}/LumEngine/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "${CMAKE_SOURCE_DIR}/LumEngine/*.hpp" "${CMAKE_SOURCE_DIR}/LumEngine/*.h")
file(GLOB_RECURSE ENGINE_C       "${CMAKE_SOURCE_DIR}/LumEngine/*.c")
file(GLOB_RECURSE ENGINE_IPP     "${CMAKE_SOURCE_DIR}/LumEngine/*.ipp")
file(GLOB_RECURSE ENGINE_SHADERS "${CMAKE_SOURCE_DIR}/LumEngine/*.vert" "${CMAKE_SOURCE_DIR}/LumEngine/*.frag")

add_executable(LumEngine 
    ${ENGINE_CPP} 
    ${ENGINE_HEADERS} 
    ${ENGINE_C} 
    ${ENGINE_IPP}
    ${ENGINE_SHADERS}
)

add_dependencies(LumEngine RunLHC)

set_source_files_properties(${ENGINE_C} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

source_group(TREE ${CMAKE_SOURCE_DIR}/LumEngine FILES 
    ${ENGINE_CPP} 
    ${ENGINE_C} 
    ${ENGINE_HEADERS} 
    ${ENGINE_SHADERS}
    ${ENGINE_IPP}
)

target_precompile_headers(LumEngine PRIVATE 
    "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/core/core_pch.hpp"
    "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/modules/ahi/ahi_pch.hpp"
    "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/modules/rhi/rhi_pch.hpp"
)

target_include_directories(LumEngine PRIVATE
    "${CMAKE_SOURCE_DIR}/LumEngine/editor/include"
    "${CMAKE_SOURCE_DIR}/LumEngine/engine/include/modules"
    "${CMAKE_SOURCE_DIR}/LumEngine/engine/include"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/fmod/inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/glfw/inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/imgui/inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/assimp/inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/stb/inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/json"
)

target_link_libraries(LumEngine PRIVATE 
    "${CMAKE_SOURCE_DIR}/LumEngine/external/fmod/lib/fmod_vc.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/glfw/lib/glfw3.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/glfw/lib/glfw3_mt.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/glfw/lib/glfw3dll.lib"
    "${CMAKE_SOURCE_DIR}/LumEngine/external/assimp/lib/assimp-vc143-mt.lib"
)