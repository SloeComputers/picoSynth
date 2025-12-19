#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(picoSynth
        LANGUAGES CXX C ASM
        VERSION 0.0)

set(HARDWARE picoX7)

add_subdirectory(Hardware)
add_subdirectory(Source)
