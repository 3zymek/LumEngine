cmake_minimum_required(VERSION 3.20)
project(LumEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(GLFW_INCLUDE_NONE)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
endif()

if(APPLE)
    set( DYLIB_DIR "${CMAKE_SOURCE_DIR}/LumEngine/External/dylib" )
    add_compile_options(-fcolor-diagnostics)
endif()

# ── QT CONFIGURATION ──────────────────────────────────────────────────────────
if(WIN32)
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/LumEngine/External/Qt/6.6.3/msvc2019_64/lib/cmake/Qt6")
elseif(APPLE)
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/LumEngine/External/Qt/6.11.2/lib/cmake/Qt6")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

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

# ── ENGINE ───────────────────────────────────────────────────────────────────
file(GLOB_RECURSE ENGINE_CPP        "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Src/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/*.hpp"
                                    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/*.h"
                                    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/*.ipp"
                                    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/*.cpp"
                                    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/*.c")

file(GLOB_RECURSE EXTERNAL_C        "${CMAKE_SOURCE_DIR}/LumEngine/External/*.c")
list(FILTER EXTERNAL_C EXCLUDE REGEX "/__MACOSX/")
list(FILTER EXTERNAL_C EXCLUDE REGEX "/\\._")

file(GLOB_RECURSE EXTERNAL_CPP_IMPL "${CMAKE_SOURCE_DIR}/LumEngine/External/Stb/Src/*.cpp")
file(GLOB_RECURSE ENGINE_C          "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Src/*.c")
file(GLOB_RECURSE ENGINE_IPP        "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Src/*.ipp")
file(GLOB_RECURSE ENGINE_SHADERS    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/*.vert"
                                    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/*.frag")

add_library(LumEngine STATIC
    ${ENGINE_CPP}
    ${ENGINE_HEADERS}
    ${ENGINE_C}
    ${ENGINE_IPP}
    ${ENGINE_SHADERS}
    ${EXTERNAL_C}
    ${EXTERNAL_CPP_IMPL}
)

add_dependencies(LumEngine RunLHC)

set_source_files_properties(${ENGINE_C}          PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(${EXTERNAL_C}        PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(${EXTERNAL_CPP_IMPL} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

source_group(TREE "${CMAKE_SOURCE_DIR}/LumEngine/Engine" FILES
    ${ENGINE_CPP} ${ENGINE_C} ${ENGINE_HEADERS} ${ENGINE_SHADERS} ${ENGINE_IPP}
)

target_include_directories(LumEngine PUBLIC
    "${CMAKE_SOURCE_DIR}/LumEngine"
    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include"
    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/Modules"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Fmod/Inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Glfw/Inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Assimp/Inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Stb/Inc"
    "${CMAKE_SOURCE_DIR}/LumEngine/External/Vulkan/Inc"
)

target_precompile_headers(LumEngine PRIVATE
    "${CMAKE_SOURCE_DIR}/LumEngine/Engine/Include/Core/CorePch.hpp"
)

if(WIN32)
    set( FMOD_LIB_DIR "${CMAKE_SOURCE_DIR}/LumEngine/External/Fmod/Lib/fmod_vc.lib" )
elseif(APPLE)
    set( FMOD_LIB_DIR "${DYLIB_DIR}/libfmod.dylib" )
endif()

if(WIN32)
    set( GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/LumEngine/External/Glfw/Lib/glfw3.lib" )
elseif(APPLE)
    set( GLFW_LIB_DIR "${DYLIB_DIR}/libglfw.3.dylib" )
endif()

if(WIN32)
    set( ASSIMP_LIB_DIR "${CMAKE_SOURCE_DIR}/LumEngine/External/Assimp/Lib/assimp-vc143-mt.lib" )
elseif(APPLE)
    set( ASSIMP_LIB_DIR "${DYLIB_DIR}/libassimp.6.0.5.dylib" )
endif()

target_link_libraries(LumEngine PRIVATE
    ${FMOD_LIB_DIR}
    ${GLFW_LIB_DIR}
    ${ASSIMP_LIB_DIR}
)

# ── EDITOR ────────────────────────────────────────────────────────────────────
file(GLOB_RECURSE EDITOR_CPP     "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Src/*.cpp")

file(GLOB_RECURSE EDITOR_HEADERS "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Include/*.hpp"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Include/*.h"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Include/*.ipp")

file(GLOB_RECURSE EDITOR_UI      "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Include/*.ui"
                                 "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Src/*.ui")

add_executable(LumEngineEditor
    ${EDITOR_CPP}
    ${EDITOR_HEADERS}
    ${EDITOR_UI}
)

add_dependencies(LumEngineEditor LumEngine)

target_compile_definitions(LumEngineEditor PRIVATE 
    QT_NO_OPENGL_HEADERS
    QT_NO_OPENGL
    GLFW_INCLUDE_NONE
)

source_group(TREE "${CMAKE_SOURCE_DIR}/LumEngine/Editor" FILES
    ${EDITOR_CPP} ${EDITOR_HEADERS} ${EDITOR_UI}
)

target_include_directories(LumEngineEditor PRIVATE
    "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Include"
    "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Include/Modules"
)

target_precompile_headers(LumEngineEditor PRIVATE
    "${CMAKE_SOURCE_DIR}/LumEngine/Editor/Include/EditorPch.hpp"
)

target_link_libraries(LumEngineEditor PRIVATE
    LumEngine
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

if(APPLE)
    set_target_properties(LumEngine PROPERTIES
        BUILD_RPATH "${DYLIB_DIR}"
    )

    set_target_properties(LumEngineEditor PROPERTIES
        BUILD_RPATH "${DYLIB_DIR}"
    )
endif()

# ── POST-BUILD COPY QT DLLs ──────────────────────────────────────────────────
if(WIN32)
    get_target_property(QT_WIDGETS_LIB Qt6::Widgets LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_WIDGETS_LIB}" DIRECTORY)
    get_filename_component(QT_PLUGINS_DIR "${QT_BIN_DIR}/../plugins" ABSOLUTE)

    add_custom_command(TARGET LumEngineEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${QT_PLUGINS_DIR}/platforms"
            "$<TARGET_FILE_DIR:LumEngineEditor>/platforms"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Qt6::Core>
            $<TARGET_FILE:Qt6::Gui>
            $<TARGET_FILE:Qt6::Widgets>
            $<TARGET_FILE_DIR:LumEngineEditor>
        COMMENT "Copying Qt binaries and platform plugins to output directory..."
    )
endif()