# CMakeList.txt : CMake project for Ectoplasme, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif() 

project("Ectoplasme"
  VERSION 0.1.0 # any version number
  LANGUAGES CXX C # programming languages used by the project
)

# Add external header
set(GLM_DIR dependencies/glm)
include_directories(${GLM_DIR} ..)

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
    set_target_properties(App PROPERTIES
        XCODE_GENERATE_SCHEME ON
        XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE "Metal")
endif()

# Add external source that have there own CmakeLists
add_subdirectory(dependencies/glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)


