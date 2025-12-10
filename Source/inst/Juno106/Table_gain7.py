#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# pylint: disable=bad-indentation

# Attenuation table...
# 0x00 => is -20 dB
# 0x40 => is   0 dB  0xFFFF
# 0x7F => is +20 dB

import math
import table

DB_RANGE = 20

def amp_fn(x):
   return math.pow(10, (DB_RANGE / 20) * (x - 1))

table.gen('gain7',
          func      = lambda i,x : amp_fn(x * 2),
          typename  = 'float',
          log2_size = 7,
          fmt       = 'f',
          is_const  = True)
