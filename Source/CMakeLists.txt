#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_subdirectory(SIG)
add_subdirectory(inst)

add_picoX7_executable(picoSynth
                         picoSynth.cpp
                      LIBRARIES
                         Juno106 Simple VL1 SIG)
