cmake_minimum_required(VERSION 3.9)
include(CMakeDependentOption)

set(SMOL_VERSION_MAJOR 0)
set(SMOL_VERSION_MINOR 0)
set(SMOL_VERSION_PATCH 1)
set(SMOL_VERSION_TWEAK 0)
set(SMOL_VERSION ${SMOL_VERSION_MAJOR}.${SMOL_VERSION_MINOR}.${SMOL_VERSION_PATCH}.${SMOL_VERSION_TWEAK})
cmake_policy(SET CMP0048 NEW)

project("smol" LANGUAGES CXX VERSION ${SMOL_VERSION})

find_package(OpenGL REQUIRED)

# Configurações gerais do Compilador
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Plataformas suportadas
set(PLATFORM win64 CACHE STRING "Target platform" FORCE)
set_property(CACHE PLATFORM PROPERTY STRINGS win64)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_compile_options(/W3 /WX)
endif()

set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Diretório para arquivos criados por geradores de configução unico
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/smolSDK/lib
    CACHE PATH "Diretorio para bibliotecas estaticas.")
   

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/smolSDK/lib
    CACHE PATH "Diretorio para bibliotecas compartilhadas.")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/smolSDK/bin
    CACHE PATH "Diretorio para executaveis e DLL's.")

if(CMAKE_CONFIGURATION_TYPES)
  # Directory output files for multi config generators
  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/smolSDK/bin" CACHE PATH "" FORCE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/smolSDK/lib" CACHE PATH "" FORCE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/smolSDK/lib" CACHE PATH "" FORCE)
  endforeach()
else()
  # Directory output files for single config generators
  string(TOUPPER ${CMAKE_BUILD_TYPE} OUTPUTCONFIG)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/smolSDK/bin" CACHE PATH "" FORCE)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/smolSDK/lib" CACHE PATH "" FORCE)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/smolSDK/lib" CACHE PATH "" FORCE)
endif()

# smol_platform.dll: Implementação de código dependente do sistema operacional da camada de plataforma
add_library(smol_platform SHARED 
    "${SOURCE_PATH}/${PLATFORM}/smol_platform_${PLATFORM}.cpp" 
    ${SOURCE_PATH}/include/smol/smol_platform.h)

target_include_directories(smol_platform PUBLIC "${SOURCE_PATH}/include")
target_include_directories(smol_platform PUBLIC "${SOURCE_PATH}/include/smol")
target_compile_options(smol_platform PRIVATE -DSMOL_PLATFORM_IMPLEMENTATION)
target_link_libraries(smol_platform PRIVATE ${OPENGL_LIBRARY})


# Gerar o arquivo smol_version.h correto
configure_file(${SOURCE_PATH}/include/smol/smol_version.h.in ${SOURCE_PATH}/include/smol/smol_version.h)

# adicionar arquivo de recursos do windows apenas se for buildar para windows
if("${PLATFORM}" STREQUAL win64)
    set(SMOL_WINDOWS_RESOURCE_FILE "${SOURCE_PATH}/win64/smol_resource_win64.rc")
endif()

# smol.dll: Smol engine
add_library(smol SHARED 
    ${SOURCE_PATH}/smol_engine.cpp
    ${SOURCE_PATH}/include/smol/smol_engine.h
    ${SOURCE_PATH}/include/smol/smol.h
    ${SOURCE_PATH}/include/smol/smol_version.h
    "${SMOL_WINDOWS_RESOURCE_FILE}")

set_target_properties(smol PROPERTIES VERSION ${SMOL_VERSION})
target_compile_options(smol PRIVATE -DSMOL_ENGINE_IMPLEMENTATION)
target_include_directories(smol PUBLIC 
    "${SOURCE_PATH}/include"
    "${SOURCE_PATH}/include/smol")
target_link_libraries(smol PRIVATE smol_platform)

add_custom_command(TARGET smol POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/smolSDK/include
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_PATH}/include/smol/ ${CMAKE_BINARY_DIR}/smolSDK/include)


# slauncher.exe: Editor / Game launcher
add_executable(slauncher WIN32 ${SOURCE_PATH}/smol_launcher.cpp "${SMOL_WINDOWS_RESOURCE_FILE}")
target_link_libraries(slauncher PRIVATE smol smol_platform)

# Make windows Debug builds to have a console subsystem
if(CMAKE_CONFIGURATION_TYPES)
  set_property(TARGET slauncher PROPERTY LINK_OPTIONS
    $<$<CXX_COMPILER_ID:MSVC>:$<IF:$<CONFIG:Release>,/SUBSYSTEM:WINDOWS,/SUBSYSTEM:CONSOLE>>)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release" )
  if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_property(TARGET slauncher PROPERTY LINK_OPTIONS /SUBSYSTEM:WINDOWS)
  endif()
endif()
