#------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(Acorn INTERFACE)

add_subdirectory(Proton)

target_link_libraries(Acorn INTERFACE
                      Proton)
