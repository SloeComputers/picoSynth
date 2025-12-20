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
      volume  = 99;
      balance = 0;
      octave  = 1;
      tune    = 0;
   }

   unsigned volume;  //<! The overall output level [0..99]
   signed   balance; //<! Balance between rhythm and melody [-50..+50]
   signed   tune;    //<! Tuning [-50..+50]
   uint8_t  octave;  //<! The octave [0..+2]
};

} // namespace VL1
