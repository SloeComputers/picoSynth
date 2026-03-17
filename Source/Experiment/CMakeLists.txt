#------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_subdirectory(Simple)
add_subdirectory(PhysM)
add_subdirectory(Additive)

add_library(Experiment INTERFACE)

target_link_libraries(Experiment INTERFACE
                      Simple PhysM Additive)
