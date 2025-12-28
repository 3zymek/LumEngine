cmake_minimum_required(VERSION 3.20)
project(LumEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

<<<<<<< HEAD
file(GLOB_RECURSE ENGINE_SOURCES "${CMAKE_SOURCE_DIR}/lumengine/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "${CMAKE_SOURCE_DIR}/lumengine/*.hpp")
file(GLOB_RECURSE ENGINE_IPP "${CMAKE_SOURCE_DIR}/lumengine/*.ipp")
file(GLOB_RECURSE ENGINE_C "${CMAKE_SOURCE_DIR}/lumengine/*.c")
file(GLOB_RECURSE ENGINE_H "${CMAKE_SOURCE_DIR}/lumengine/*.h")

add_executable(LumEngine ${ENGINE_SOURCES} ${ENGINE_HEADERS} ${ENGINE_IPP} ${ENGINE_H} ${ENGINE_C})

foreach(src ${ENGINE_SOURCES} ${ENGINE_HEADERS} ${ENGINE_IPP} ${ENGINE_H} ${ENGINE_C})
    file(RELATIVE_PATH rel_path "${CMAKE_SOURCE_DIR}/lumengine/engine" "${src}")
    get_filename_component(folder ${rel_path} PATH)
    string(REPLACE "/" "\\" folder "${folder}")
    source_group("${folder}" FILES "${src}")
endforeach(src ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(LumEngine PUBLIC
    "${CMAKE_SOURCE_DIR}/lumengine/engine"
    "${CMAKE_SOURCE_DIR}/lumengine/engine/core"
    "${CMAKE_SOURCE_DIR}/lumengine/engine/modules"
    "${CMAKE_SOURCE_DIR}/lumengine/engine/modules/entitymodule"
    "${CMAKE_SOURCE_DIR}/lumengine/engine/modules/eventmodule"
    "${CMAKE_SOURCE_DIR}/lumengine/engine/modules/audiomodule"
    "${CMAKE_SOURCE_DIR}/lumengine/external/fmod/inc"
    "${CMAKE_SOURCE_DIR}/lumengine/external/glfw/inc"
    "${CMAKE_SOURCE_DIR}/lumengine/external/imgui/inc"
    "${CMAKE_SOURCE_DIR}/lumengine/external/imgui/src"
    )
target_link_directories(LumEngine PRIVATE "${CMAKE_SOURCE_DIR}/lumengine/external/fmod/lib")
target_link_directories(LumEngine PRIVATE "${CMAKE_SOURCE_DIR}/lumengine/external/glfw/lib")
target_link_libraries(LumEngine PRIVATE "fmod_vc.lib")
target_link_libraries(LumEngine PRIVATE "glfw3.lib")
target_link_libraries(LumEngine PRIVATE "glfw3_mt.lib")
target_link_libraries(LumEngine PRIVATE "glfw3dll.lib")
=======
if(MSVC)
    add_compile_options(/Bt)
endif()

file(GLOB_RECURSE ENGINE_CPP "${CMAKE_SOURCE_DIR}/lumengine/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "${CMAKE_SOURCE_DIR}/lumengine/*.hpp" "${CMAKE_SOURCE_DIR}/lumengine/*.h")
file(GLOB_RECURSE ENGINE_C "${CMAKE_SOURCE_DIR}/lumengine/*.c")

add_executable(LumEngine ${ENGINE_CPP} ${ENGINE_HEADERS} ${ENGINE_C})

set_source_files_properties(${ENGINE_C} ${EXTERNAL_C} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

target_sources(LumEngine PRIVATE ${ENGINE_CPP} ${ENGINE_C} ${ENGINE_HEADERS})

source_group(TREE ${CMAKE_SOURCE_DIR}/lumengine FILES ${ENGINE_CPP} ${ENGINE_C} ${ENGINE_HEADERS})

target_precompile_headers(LumEngine PRIVATE ${CMAKE_SOURCE_DIR}/lumengine/engine/include/core/core_pch.hpp)
target_precompile_headers(LumEngine PRIVATE ${CMAKE_SOURCE_DIR}/lumengine/engine/include/modules/audio/audio_pch.hpp)

target_include_directories(LumEngine PRIVATE
    "${CMAKE_SOURCE_DIR}/lumengine/engine/include/modules"
    "${CMAKE_SOURCE_DIR}/lumengine/engine/include"
    "${CMAKE_SOURCE_DIR}/lumengine/external/fmod/inc"
    "${CMAKE_SOURCE_DIR}/lumengine/external/glfw/inc"
    "${CMAKE_SOURCE_DIR}/lumengine/external/imgui/inc"
)

target_link_libraries(LumEngine PRIVATE 
"${CMAKE_SOURCE_DIR}/lumengine/external/fmod/lib/fmod_vc.lib"
"${CMAKE_SOURCE_DIR}/lumengine/external/glfw/lib/glfw3.lib"
"${CMAKE_SOURCE_DIR}/lumengine/external/glfw/lib/glfw3_mt.lib"
"${CMAKE_SOURCE_DIR}/lumengine/external/glfw/lib/glfw3dll.lib"
)
>>>>>>> dd5bcd8 (reworking events)
