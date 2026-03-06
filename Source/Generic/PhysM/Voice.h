//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/SIG.h"

#include "NoEffect.h"
#include "Patch.h"

namespace PhysM {

class Voice
{
public:
   Voice()
   {
      exciter.gain = 0.5;
   }

   void program(const Patch* patch_)
   {
      exciter.setFreq(patch_->exciter_freq);
      exciter.setWidth(patch_->exciter_length);
      feedback = patch_->feedback;
      delay.setLengthT(patch_->delay * 0.001);
      volume = SIG::dBGainLookup(patch_->volume);

      exciter_filter.setFreq(patch_->exciter_cutoff);
      delay_filter.setFreq(patch_->delay_cutoff);
   }

   void noteOn(uint8_t note_, uint8_t velocity_)
   {
   }

   void noteOff(uint8_t velocity_)
   {
   }

   void noteBend(int16_t bend_) {}

   void tick(const NoEffect& effect_, unsigned n_)
   {
   }

   SIG::Signal sample(const NoEffect& effect_)
   {
      SIG::Signal signal = noise() * (exciter() + 0.5);

      SIG::Signal out = delay_filter(delay());

      delay = exciter_filter(signal) + feedback(out);

      return volume(out);
   }

private:
   SIG::Osc::Noise      noise{};
   SIG::Osc::Pwm        exciter{};
   SIG::Filter::OnePole exciter_filter{SIG::Filter::LOPASS};
   SIG::DelayV<1000>    delay{};
   SIG::Filter::OnePole delay_filter{SIG::Filter::LOPASS};
   SIG::Gain            feedback{};
   SIG::Gain            volume{};
};

} // namespace PhysM
