//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/SIG.h"

#include "Effect.h"
#include "Patch.h"

namespace AcornProton {

class Voice
{
public:
   Voice() = default;

   void program(const Patch* patch_)
   {
   }

   void noteOn(uint8_t note_, uint8_t velocity_)
   {
      tone.setNote(note_);
      tone.sync();

      gain = 1.0;
   }

   void noteOff(uint8_t velocity_)
   {
      gain = 0.0;
   }

   void tick(const Effect& effect_, unsigned n_) {}

   SIG::Signal sample(const Effect& effect_)
   {
      SIG::Signal signal;

      switch(mode)
      {
      case TONE:  signal = tone();  break;
      case NOISE: signal = noise(); break;
      }

      return gain(signal);
   }

private:
   enum Mode { TONE, NOISE };

   Mode             mode;
   SIG::Osc::Square tone;
   SIG::Osc::Noise  noise;
   SIG::Gain        gain{0.0};
};

} // namespace AcornProton
