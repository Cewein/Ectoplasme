# CMakeList.txt : CMake project for Ectoplasme, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
include(FetchContent)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif() 

project("Ectoplasme"
  VERSION 0.1.0 # any version number
  LANGUAGES CXX C # programming languages used by the project
)

# Add source to this project's executable.
add_executable (Ectoplasme "Ectoplasme.cpp" )

set_target_properties(Ectoplasme PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF
  COMPILE_WARNING_AS_ERROR ON
)

if (MSVC)
    target_compile_options(Ectoplasme PRIVATE /W4)
else()
    target_compile_options(Ectoplasme PRIVATE -Wall -Wextra -pedantic)
endif()

if(XCODE)
    set_target_properties(Ectoplasme PROPERTIES
        XCODE_GENERATE_SCHEME ON
        XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE "Metal")
endif()

#load webgpu from elie's repo
FetchContent_Declare(
  webgpu
  GIT_REPOSITORY https://github.com/eliemichel/WebGPU-distribution
  GIT_TAG        main
)
FetchContent_MakeAvailable(webgpu)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG        master
)
FetchContent_MakeAvailable(glfw)

# FetchContent for GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG master
)
FetchContent_MakeAvailable(glm)

target_link_libraries(Ectoplasme PRIVATE glfw webgpu)


