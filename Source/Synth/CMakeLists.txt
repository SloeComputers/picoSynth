#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(Synth INTERFACE)

target_include_directories(Synth INTERFACE .)

target_link_libraries(Synth INTERFACE MIDI SIG)
