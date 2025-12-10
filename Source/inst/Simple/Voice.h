//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/SineOsc.h"
#include "SIG/Gain.h"

namespace Simple {

class Voice
{
public:
   Voice() = default;

   void noteOn(uint8_t note_, uint8_t velocity_)
   {
      osc.setNote(note_);
      osc.sync();

      gain = 1.0;
   }

   void noteOff(uint8_t velocity_)
   {
      gain = 0.0;
   }

   Sample sample()
   {
      return gain(osc());
   }

   static Sample effect(Sample sample_)
   {
      return sample_;
   }

private:
   SineOsc osc{};
   Gain    gain{0.0};
};

} // namespace Simple
