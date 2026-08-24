#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(picoSynth
        LANGUAGES CXX C ASM
        VERSION 0.0
        HOMEPAGE_URL "https://github.com/SloeComputers/picoSynth")

set(PDK_PROJ_OWNER "Sloe Computers")
set(PDK_PROJ_EMAIL "sloecomputer@gmail.com")

if(NOT DEFINED BUILD_TESTING)
    set(BUILD_TESTING OFF CACHE BOOL "Build unit tests")
endif()
include(CTest)

set(HWR_CONFIG picoX7)

add_subdirectory(PDK)
add_subdirectory(Source)
