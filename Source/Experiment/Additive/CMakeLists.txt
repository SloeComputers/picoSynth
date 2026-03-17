#------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(Additive INTERFACE)

target_include_directories(Additive INTERFACE ..)

target_link_libraries(Additive INTERFACE Synth)
