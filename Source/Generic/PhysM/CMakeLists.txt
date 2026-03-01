#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(PhysM INTERFACE)

target_include_directories(PhysM INTERFACE ..)

target_link_libraries(PhysM INTERFACE Synth)
