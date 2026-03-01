#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(Casio INTERFACE)

add_subdirectory(VL1)

target_link_libraries(Casio INTERFACE
                      VL1)
