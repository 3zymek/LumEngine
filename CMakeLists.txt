cmake_minimum_required(VERSION 3.20)
project(LumEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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