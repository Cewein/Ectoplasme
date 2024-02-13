# CMakeList.txt : CMake project for Ectoplasme, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)
project(Ectoplasme VERSION 0.1.0 LANGUAGES CXX C)

include(FetchContent)

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set source files for the executable.
add_executable(Ectoplasme "Ectoplasme.cpp" "src/wgpucallback.cpp" "src/wgpucallback.h" "src/wgpu.cpp" "src/wgpu.h")

set_target_properties(Ectoplasme PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF
  COMPILE_WARNING_AS_ERROR ON
)

# Compiler-specific options
if(MSVC)
  target_compile_options(Ectoplasme PRIVATE /W4)
else()
  target_compile_options(Ectoplasme PRIVATE -Wall -Wextra -pedantic)
endif()

# Xcode-specific options
if(XCODE)
  set_target_properties(Ectoplasme PROPERTIES
    XCODE_GENERATE_SCHEME ON
    XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE "Metal"
  )
endif()

# FetchContent for external dependencies
# WebGPU is loaded through Elie's git repo
# Allow both Webgpu-native.h and Dawn to be used.
# Default : webgpu-native
FetchContent_Declare(
  webgpu
  GIT_REPOSITORY https://github.com/eliemichel/WebGPU-distribution
  GIT_TAG main
)
FetchContent_MakeAvailable(webgpu)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG master
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG master
)
FetchContent_MakeAvailable(glm)

# GLFW does not support yet webGPU, thus custom lib
#to change in case of official glfw support of webGPU
FetchContent_Declare(
  glfw3webgpu
  GIT_REPOSITORY https://github.com/eliemichel/glfw3webgpu
  GIT_TAG main
)

FetchContent_Populate(glfw3webgpu)
FetchContent_GetProperties(glfw3webgpu)
add_subdirectory(${glfw3webgpu_SOURCE_DIR})

# Link libraries
target_link_libraries(Ectoplasme PRIVATE glfw webgpu glfw3webgpu)

#copy pre-computed files
target_copy_webgpu_binaries(Ectoplasme)
