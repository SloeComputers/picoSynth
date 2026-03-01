#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# pylint: disable=bad-indentation

import math
import table

SAMPLE_RATE = 48000  # XXX Must match the value in Sample.h

def index2freq(index, _):
   """ Convert table index to freq (Hz) """

   A4_FREQ              = 440   # Freq of the A above middle C (Hz)
   MIDI_A4_NOTE         = 69    # MIDI note value for the A above middle C
   SEMITONES_PER_OCTAVE = 12

   exp   = (index - MIDI_A4_NOTE) / SEMITONES_PER_OCTAVE
   freq  = A4_FREQ * math.pow(2, exp)
   return freq

table.gen('freq7',
          func      = index2freq,
          typename  = 'float',
          log2_size = 7,
          fmt       = 'f',
          is_const  = False)
