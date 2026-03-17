//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/SIG.h"

#include "NoEffect.h"
#include "Patch.h"

namespace Additive {

class Voice
{
public:
   Voice() = default;

   void program(const Patch* patch_)
   {
      for(unsigned i = 0; i < Patch::NUM_HARMONIC; ++i)
      {
         const Patch::Harmonic& h = patch_->harm[i];

         sine[i].gain = h.level;
         env[i].setAttack_mS(unsigned(h.attack * 1000));
         env[i].setDecay_mS(unsigned(h.decay * 4000));
         env[i].setSustain(0.0f);
         env[i].setRelease_mS(unsigned(h.release * 4000));
      }

      volume = SIG::dBGainLookup(patch_->volume);
   }

   void noteOn(uint8_t note_, uint8_t velocity_)
   {
      for(unsigned i = 0; i < Patch::NUM_HARMONIC; ++i)
      {
         sine[i].sync();
         sine[i].setNote(note_ + i * 12);
         env[i].on();
      }
   }

   void noteOff(uint8_t velocity_)
   {
      for(unsigned i = 0; i < Patch::NUM_HARMONIC; ++i)
      {
         env[i].off();
      }
   }

   void noteBend(int16_t bend_) {}

   void tick(const NoEffect& effect_, unsigned n_) {}

   SIG::Signal sample(const NoEffect& effect_)
   {
      SIG::Signal out{};

      for(unsigned i = 0; i < Patch::NUM_HARMONIC; ++i)
      {
         out += sine[i]() * env[i]();
      }

      return volume(out);
   }

private:
   SIG::Osc::Sine sine[Patch::NUM_HARMONIC];
   SIG::Env::Adsr env[Patch::NUM_HARMONIC];
   SIG::Gain      volume{};
};

} // namespace Additive
