//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/SIG.h"

#include "Effect.h"
#include "Patch.h"

namespace Simple {

class Voice
{
public:
   Voice() = default;

   void program(const Patch* patch_)
   {
      osc.gain = patch_->value / 127.0f;
   }

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

   Signal sample(const Effect& effect_)
   {
      return gain(osc());
   }

private:
   Osc::Square osc{};
   Gain        gain{0.0};
};

} // namespace Simple
