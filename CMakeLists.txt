cmake_minimum_required(VERSION 3.20)
project(LumEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/Bt)
endif()

file(GLOB_RECURSE ENGINE_CPP "${CMAKE_SOURCE_DIR}/lumengine/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "${CMAKE_SOURCE_DIR}/lumengine/*.hpp" "${CMAKE_SOURCE_DIR}/lumengine/*.h")
file(GLOB_RECURSE ENGINE_C "${CMAKE_SOURCE_DIR}/lumengine/*.c")
file(GLOB_RECURSE ENGINE_IPP)
file(GLOB_RECURSE ENGINE_SHADERS "${CMAKE_SOURCE_DIR}/lumengine/*.vert" "${CMAKE_SOURCE_DIR}/lumengine/*.frag")

add_executable(LumEngine ${ENGINE_CPP} ${ENGINE_HEADERS} ${ENGINE_C} ${ENGINE_SHADERS} ${ENGINE_SHADERS})

set_source_files_properties(${ENGINE_C} ${EXTERNAL_C} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

target_sources(LumEngine PRIVATE ${ENGINE_CPP} ${ENGINE_C} ${ENGINE_HEADERS} ${ENGINE_SHADERS})

source_group(TREE ${CMAKE_SOURCE_DIR}/lumengine FILES ${ENGINE_CPP} ${ENGINE_C} ${ENGINE_HEADERS} ${ENGINE_SHADERS})

target_precompile_headers(LumEngine PRIVATE ${CMAKE_SOURCE_DIR}/lumengine/engine/include/core/core_pch.hpp)
target_precompile_headers(LumEngine PRIVATE ${CMAKE_SOURCE_DIR}/lumengine/engine/include/modules/audio/audio_pch.hpp)
target_precompile_headers(LumEngine PRIVATE ${CMAKE_SOURCE_DIR}/lumengine/engine/include/modules/rhi/rhi_pch.hpp)

target_include_directories(LumEngine PRIVATE
    "${CMAKE_SOURCE_DIR}/lumengine/editor/include"
    "${CMAKE_SOURCE_DIR}/lumengine/engine/include/modules"
    "${CMAKE_SOURCE_DIR}/lumengine/engine/include"
    "${CMAKE_SOURCE_DIR}/lumengine/external/fmod/inc"
    "${CMAKE_SOURCE_DIR}/lumengine/external/glfw/inc"
    "${CMAKE_SOURCE_DIR}/lumengine/external/imgui/inc"
    "${CMAKE_SOURCE_DIR}/lumengine/external/assimp/inc"
    "${CMAKE_SOURCE_DIR}/lumengine/external/stb/inc"
)

target_link_libraries(LumEngine PRIVATE 
"${CMAKE_SOURCE_DIR}/lumengine/external/fmod/lib/fmod_vc.lib"
"${CMAKE_SOURCE_DIR}/lumengine/external/glfw/lib/glfw3.lib"
"${CMAKE_SOURCE_DIR}/lumengine/external/glfw/lib/glfw3_mt.lib"
"${CMAKE_SOURCE_DIR}/lumengine/external/glfw/lib/glfw3dll.lib"
"${CMAKE_SOURCE_DIR}/lumengine/external/assimp/lib/assimp-vc143-mt.lib"
)