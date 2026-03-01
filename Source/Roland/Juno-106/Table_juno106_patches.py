#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# Import Juno-106 patches from the .106 file format used by the j106 librarian
# see http://www.jarvik7.net/juno-106/

import sys
import table

filename = sys.argv[1]

data = []

with open(filename, 'rb') as file:
   while True:
      byte = file.read(1)
      if byte == b'':
         break
      data.append(int.from_bytes(byte, byteorder='big', signed=False))

offset = 0

table.gen("juno106_patches",
          func      = lambda i,x : data[i + offset],
          typename  = "uint8_t",
          log2_size = 12,
          fmt       = '02x')
