#------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(SH_101 STATIC 
            Patch.cpp)

target_include_directories(SH_101 PUBLIC ..)

target_link_libraries(SH_101 PUBLIC Synth)
