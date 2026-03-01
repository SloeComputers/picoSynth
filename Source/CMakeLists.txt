#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_subdirectory(Synth)

add_subdirectory(Acorn)
add_subdirectory(Casio)
add_subdirectory(Roland)
add_subdirectory(Generic)

add_picoX7_executable(picoSynth
                         picoSynth.cpp
                      LIBRARIES
                         Acorn Casio Roland Generic)
