cmake_minimum_required (VERSION 3.1)
set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 11)

set(APP_NAME GLEngine)
project (${APP_NAME})

#set(CORE_ROOT ${CMAKE_SOURCE_DIR}/core)
set(CORE_ROOT core)


# core
add_subdirectory(${CORE_ROOT})

if(WINDOWS)
  ADD_DEFINITIONS (-DCOMPILE_CL -D_USING_V110_SDK71_ -D_WINDOWS -DWIN32 -D_WIN32)
  ADD_DEFINITIONS (-DCC_TARGET_OS_WIN32)
elseif(MACOSX OR APPLE)
  ADD_DEFINITIONS (-DCC_TARGET_OS_MAC)
elseif(LINUX)
  ADD_DEFINITIONS(-DCC_TARGET_OS_LINUX)
elseif(ANDROID)
  ADD_DEFINITIONS(-DCC_TARGET_OS_ANDROID)
else()
  #message( FATAL_ERROR "Unsupported platform, CMake will exit" )
endif()


# Compiler options
if(MSVC)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS
                  -wd4251 -wd4244 -wd4334 -wd4005 -wd4820 -wd4710
                  -wd4514 -wd4056 -wd4996 -wd4099)
else()
  set(CMAKE_C_FLAGS_DEBUG "-g -Wall -DCOCOS2D_DEBUG=1")
  set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations -Wno-reorder")
  if(CLANG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
endif(MSVC)


set(GAME_SRC
  runtime/proj.win32/Main.cpp
  runtime/Classes/AppDelegate.cpp
)


# È«¾ÖËÑË÷Â·¾¶
#list(APPEND CMAKE_INCLUDE_PATH "include_path")
#list(APPEND CMAKE_LIBRARY_PATH "lib_path")

include_directories(
  runtime/Classes
  external/opengl1.1/include
  ${CORE_ROOT}
  ${CORE_ROOT}/base
  ${CORE_ROOT}/framework
  ${CORE_ROOT}/texture
  ${CORE_ROOT}/include
)


LINK_DIRECTORIES("external/opengl1.1/lib/win32")
LINK_LIBRARIES("Core")

add_executable(${APP_NAME} ${GAME_SRC})
target_link_libraries(${APP_NAME} Core)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")




