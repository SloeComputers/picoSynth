#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

autogen_py(Table_amp7)
autogen_py(Table_gain7)

add_library(Juno106 STATIC
            Patch.cpp
            SysEx.cpp
            Table_amp7.cpp
            Table_gain7.cpp)

target_include_directories(Juno106 PUBLIC .. ${CMAKE_CURRENT_BINARY_DIR})
