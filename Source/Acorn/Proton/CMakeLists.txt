#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(Proton INTERFACE)

target_include_directories(Proton INTERFACE ..)

target_link_libraries(Proton INTERFACE Synth) 
