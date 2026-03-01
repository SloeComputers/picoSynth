#------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(TB_303 STATIC 
            Patch.cpp)

target_include_directories(TB_303 PUBLIC ..)

target_link_libraries(TB_303 PUBLIC Synth)
