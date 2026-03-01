//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace VL1 {

struct Control
{
   Control()
   {
      init();
   }

   void init()
   {
      gain    = 0;
      balance = 0;
      octave  = 1;
      tune    = 0;
      debug   = 0;
   }

   unsigned gain;    //<! The overall output gain [-30..0] (dB)
   signed   balance; //<! Balance between rhythm and melody [-50..+50]
   signed   tune;    //<! Tuning [-50..+50]
   uint8_t  octave;  //<! The octave [0..+2]
   unsigned debug;   //<! Debug control
};

} // namespace VL1
