#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# Import Juno-106 patches from the .106 file format used by the j106 librarian
# see http://www.jarvik7.net/juno-106/

import sys

filename = 'factory_patches.106' # sys.argv[1]

image = []

with open(filename, 'rb') as file:
   while True:
      byte = file.read(1)
      if byte == b'':
         break
      image.append(int.from_bytes(byte, byteorder='big', signed=False))

def read32():
   global offset
   value = image[offset]
   offset += 1
   value = (value << 8) + image[offset]
   offset += 1
   value = (value << 8) + image[offset]
   offset += 1
   value = (value << 8) + image[offset]
   offset += 1
   return value

def readString():
   global offset
   size = read32()
   text = ''
   for i in range(size):
      text += chr(image[offset])
      offset += 1

   return text

offset = 6
file_type = readString()
offset += 8
file_name = readString()
offset += 4
patches = read32()

print(file_type)
print(file_name)
print(patches)

for i in range(patches):
   name = readString()
   ident = readString()
   pass

