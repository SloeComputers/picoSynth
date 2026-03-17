//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/SIG.h"

#include "NoEffect.h"
#include "Patch.h"

namespace Simple {

class Voice
{
public:
   Voice()
   {
      env.setAttack_mS(0);
      env.setDecay_mS(100000.0);
      env.setSustain(uint8_t(0));
      env.setSustain_mS(0);
      env.setRelease_mS(10.0);

      osc.setFreq(440.0f);
      poly5_clip.setN(0.0f);
   }

   void program(const Patch* patch_)
   {
      clip  = patch_->clip;
      gain  = patch_->gain;
      drive = patch_->drive;

      osc.setNote(patch_->f_coarse);
      poly5_clip.setN(patch_->n);
   }

   void noteOn(uint8_t note_, uint8_t velocity_)
   {
      osc.setNote(note_);
      osc.sync();
      env.on();
   }

   void noteOff(uint8_t velocity_)
   {
      env.off();
   }

   void noteBend(int16_t bend_) {}

   void tick(const NoEffect& effect_, unsigned n_) {}

   SIG::Signal sample(const NoEffect& effect_)
   {
      SIG::Signal out = gain(osc()) * env();

      switch(clip)
      {
      case 0: out = no_clip(out);    break;
      case 1: out = hard_clip(out);  break;
      case 2: out = poly_clip(out);  break;
      case 3: out = poly5_clip(out); break;
      case 4: out = tanh_clip(out, drive); break;
      }

      return out;
   }

private:
   SIG::Float       drive{};
   uint8_t          clip{};
   SIG::Osc::Sine   osc{};
   SIG::Env::Adsr   env{};
   SIG::Gain        gain{};

   SIG::Clip::No    no_clip{};
   SIG::Clip::Hard  hard_clip{};
   SIG::Clip::Poly  poly_clip{};
   SIG::Clip::Poly5 poly5_clip{};
   SIG::Clip::Tanh  tanh_clip{};
};

} // namespace Simple
