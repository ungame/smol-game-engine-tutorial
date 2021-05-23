cmake_minimum_required(VERSION 3.18)
include(CMakeDependentOption)
project("smol" LANGUAGES CXX)

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



# Diretório para arquivos criados por multi geradores de configução
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/smolSDK/bin" CACHE PATH "" FORCE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/smolSDK/lib" CACHE PATH "" FORCE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/smolSDK/lib" CACHE PATH "" FORCE)
endforeach()


# smol_platform.dll: Implementação de código dependente do sistema operacional da camada de plataforma
add_library(smol_platform SHARED 
    "${SOURCE_PATH}/src/${PLATFORM}/smol_platform_${PLATFORM}.cpp" 
    ${SOURCE_PATH}/src/include/smol/smol_platform.h)

target_include_directories(smol_platform PUBLIC "${SOURCE_PATH}/src/include")
target_compile_options(smol_platform PRIVATE -DSMOL_PLATFORM_IMPLEMENTATION)

# adicionar arquivo de recursos do windows apenas se for buildar para windows
if("${PLATFORM}" STREQUAL win64)
    set(SMOL_WINDOWS_RESOURCE_FILE "${SOURCE_PATH}/src/win64/smol_resource_win64.rc")
endif()

# smol.dll: Smol engine
add_library(smol SHARED ${SOURCE_PATH}/src/smol_engine.cpp
    ${SOURCE_PATH}/src/include/smol/smol_engine.h
    ${SOURCE_PATH}/src/include/smol/smol.h
    "${SMOL_WINDOWS_RESOURCE_FILE}")

target_include_directories(smol PUBLIC "${SOURCE_PATH}/src/include")
target_compile_options(smol PRIVATE -DSMOL_ENGINE_IMPLEMENTATION)
target_link_libraries(smol PRIVATE smol_platform)

add_custom_command(TARGET smol POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/smolSDK/include
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_PATH}/src/include/smol/ ${CMAKE_BINARY_DIR}/smolSDK/include)

# slauncher.exe: Editor / Game launcher
add_executable(slauncher WIN32 ${SOURCE_PATH}/src/smol_launcher.cpp "${SMOL_WINDOWS_RESOURCE_FILE}")
target_link_libraries(slauncher PRIVATE smol smol_platform)
