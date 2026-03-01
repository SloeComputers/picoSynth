#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

autogen_py(Table_freq7)

add_library(Juno_106 STATIC
            Patch.cpp
            SysEx.cpp
            Table_freq7.cpp)

target_include_directories(Juno_106 PUBLIC .. ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Juno_106 PUBLIC Synth)
