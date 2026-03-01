#------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(Generic INTERFACE)

add_subdirectory(Simple)
add_subdirectory(PhysM)

target_link_libraries(Generic INTERFACE
                      Simple PhysM)
