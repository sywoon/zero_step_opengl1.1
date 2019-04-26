cmake_minimum_required (VERSION 3.1)
set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 11)

set(APP_NAME GLEngine)
project (${APP_NAME})

set(CORE_ROOT ${CMAKE_SOURCE_DIR}/core)


# core
add_subdirectory(${CORE_ROOT})

if(WINDOWS)
  ADD_DEFINITIONS (-DCOMPILE_CL -D_USING_V110_SDK71_ -D_WINDOWS -DWIN32 -D_WIN32)
  ADD_DEFINITIONS (-DCC_TARGET_OS_WIN32)
  set(PLATFORM_FOLDER win32)
elseif(MACOSX OR APPLE)
  ADD_DEFINITIONS (-DCC_TARGET_OS_MAC)
  set(PLATFORM_FOLDER mac)
elseif(LINUX)
  ADD_DEFINITIONS(-DCC_TARGET_OS_LINUX)
  set(PLATFORM_FOLDER linux)
elseif(ANDROID)
  ADD_DEFINITIONS(-DCC_TARGET_OS_ANDROID)
  set(PLATFORM_FOLDER android)
else()
  #message( FATAL_ERROR "Unsupported platform, CMake will exit" )
endif()


if(MSVC)
    add_definitions("-COMPILE_CL -_CRT_SECURE_NO_WARNINGS 
                    -_USING_V110_SDK71_ 
                    -wd4819 -wd4996 -wd4018")
else()
    add_definitions("-_CRT_SECURE_NO_WARNINGS")
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


include_directories(
  runtime/Classes
  ${CORE_ROOT}
  ${CORE_ROOT}/base
  ${CORE_ROOT}/texture
  ${CORE_ROOT}/include
)

# add the executable
add_executable(${APP_NAME} ${GAME_SRC})

target_link_libraries(${APP_NAME} core)

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

set_target_properties(${APP_NAME} PROPERTIES
     RUNTIME_OUTPUT_DIRECTORY  "${APP_BIN_DIR}")


