#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(Synth STATIC
            MidiController.cpp)

target_include_directories(Synth PUBLIC .)

target_link_libraries(Synth PUBLIC SIG)
