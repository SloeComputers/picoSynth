#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(picoSynth
        LANGUAGES CXX C ASM
        VERSION 0.1)

set(HARDWARE picoX7)

add_subdirectory(Platform/Hardware)
add_subdirectory(Source)
