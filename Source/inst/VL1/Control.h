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
      volume  = 0x7F;
      balance = 0;
      octave  = 0;
      tune    = 0;
   }

   unsigned volume;  //<! The overall output level
   signed   balance; //<! Balance between rhythm and melody
   signed   octave;  //<! The octave
   signed   tune;    //<! Tuning
};

} // namespace VL1
