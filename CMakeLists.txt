cmake_minimum_required(VERSION 3.20)
project(LumEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ENGINE_SOURCES "${CMAKE_SOURCE_DIR}/lumengine/engine/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "${CMAKE_SOURCE_DIR}/lumengine/engine/*.hpp")
file(GLOB_RECURSE ENGINE_IPP "${CMAKE_SOURCE_DIR}/lumengine/engine/*.ipp")
file(GLOB_RECURSE ENGINE_C "${CMAKE_SOURCE_DIR}/lumengine/engine/*.c")
file(GLOB_RECURSE ENGINE_H "${CMAKE_SOURCE_DIR}/lumengine/engine/*.h")

add_executable(LumEngine ${ENGINE_SOURCES} ${ENGINE_HEADERS} ${ENGINE_IPP} ${ENGINE_H} ${ENGINE_C})

foreach(src ${ENGINE_SOURCES} ${ENGINE_HEADERS} ${ENGINE_IPP} ${ENGINE_H} ${ENGINE_C})
    file(RELATIVE_PATH rel_path "${CMAKE_SOURCE_DIR}/lumengine/engine" "${src}")
    get_filename_component(folder ${rel_path} PATH)
    string(REPLACE "/" "\\" folder "${folder}")
    source_group("${folder}" FILES "${src}")
endforeach(src ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(LumEngine PUBLIC
    "${CMAKE_SOURCE_DIR}/lumengine/engine"
    "${CMAKE_SOURCE_DIR}/lumengine/engine/entitymodule"
    "${CMAKE_SOURCE_DIR}/lumengine/engine/eventmodule"
    "${CMAKE_SOURCE_DIR}/lumengine/engine/audiomodule"
    "${CMAKE_SOURCE_DIR}/lumengine/external/fmod/inc"
    )
target_link_directories(LumEngine PRIVATE "${CMAKE_SOURCE_DIR}/lumengine/external/fmod/lib")
target_link_libraries(LumEngine PRIVATE "fmod_vc.lib")