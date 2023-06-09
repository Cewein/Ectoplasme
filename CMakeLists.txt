﻿# CMakeList.txt : CMake project for Ectoplasme, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif() 

project ("Ectoplasme")

#include vulkan in the project
message(STATUS "Using module to find Vulkan")

# Find Vulkan Path using CMake's Vulkan Module
find_package(Vulkan REQUIRED)

IF (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
ELSE()
	message(STATUS ${Vulkan_LIBRARY})
ENDIF()


# Add external header
set(GLM_DIR dependencies/glm)
include_directories(${GLM_DIR} ..)


# Add source to this project's executable.
add_executable (Ectoplasme "Ectoplasme.cpp" )


# Add external source that have there own CmakeLists
add_subdirectory(dependencies/glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

# Include Vulkan as a targeted lib
target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)


