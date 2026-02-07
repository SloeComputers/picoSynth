//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/SIG.h"

#include "Effect.h"
#include "Patch.h"

namespace TB_303 {

class Voice
{
public:
   Voice() = default;

   void program(const Patch* patch_)
   {
      vco_wave_is_square = patch_->vco_wave == 1;
      vco_tuning         = patch_->vco_tuning;

      vcf.setFreq(200.0f + patch_->vcf_freq * 200.0f);
      vcf.setQ(0.7 + patch_->vcf_resonance);
   }

   void noteOn(uint8_t note_, uint8_t velocity_)
   {
      vco_square.setNote(note_);
      vco_ramp.setNote(note_);

      vca = 0.1;
   }

   void noteOff(uint8_t velocity_)
   {
      vca = 0.0;
   }

   void tick(const Effect& effect_, unsigned n_)
   {
   }

   SIG::Signal sample(const Effect& effect_)
   {
      SIG::Signal vco_out = vco_wave_is_square ? vco_square(vco_tuning)
                                               : vco_ramp(vco_tuning);
      SIG::Signal vcf_out = vcf(vco_out);

#if 0
      SIG::Signal vcf_out = vcf(vco_out + fb());

      fb = fb_gain(vcf_out);

      return vca(vcf_out + fb());
#endif

      return vca(vcf_out);
   }

private:
   bool  vco_wave_is_square{false};
   float vco_tuning{0.0};

   SIG::Osc::Square        vco_square{};
   SIG::Osc::Ramp          vco_ramp{};
   SIG::Filter::DualBiQuad vcf{SIG::Filter::LOPASS};
   SIG::Delay              fb{};
   SIG::Gain               fb_gain{};
   SIG::Gain               vca{};
};

} // namespace TB_303
