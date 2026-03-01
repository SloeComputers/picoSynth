#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(Roland INTERFACE)

add_subdirectory(TB-303)
add_subdirectory(SH-101)
add_subdirectory(Juno-106)

target_link_libraries(Roland INTERFACE
                      TB_303 SH_101 Juno_106)
