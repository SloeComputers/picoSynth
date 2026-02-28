//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/SIG.h"

#include "NoEffect.h"
#include "Patch.h"
#include "Control.h"

namespace TB_303 {

class Voice
{
public:
   Voice()
   {
      vca_env.setAttack_mS(0);
      vca_env.setDecay_mS(8000.0);
      vca_env.setSustain(uint8_t(0));
      vca_env.setSustain_mS(0);
      vca_env.setRelease_mS(5.0);

      vcf_env.setAttack_mS(0);
      vcf_env.setSustain(uint8_t(0));
      vcf_env.setSustain_mS(0);
      vcf_env.setRelease_mS(5.0);
   }

   void program(const Patch* patch_)
   {
      vco_wave = patch_->vco_wave;
      vcf_freq = 100.0f + patch_->vcf_freq * 130.0f;
      env_mod  = patch_->env_mod * 200.0f;
      acc_mod  = patch_->accent * 0.1f;

      vcf1.setQ(0.707f + patch_->vcf_resonance / 3.0f);

      vcf_env.setDecay_mS(400.0f + patch_->env_decay * 150.0f);
   }

   void program(const Control* control_)
   {
      vco_tuning = control_->tuning;
      volume     = SIG::dBGainLookup(control_->volume);

      hpf.setFreq(control_->hpf_freq);
      hpf.setQ(control_->hpf_q);
   }

   void noteOn(uint8_t note_, uint8_t velocity_)
   {
      accent = velocity_ >= 100 ? acc_mod : 0.0;

      vco_square.setNote(note_);
      vco_ramp.setNote(note_);

      if (vca_env.isOff())
      {
         vco_square.sync();
         vco_ramp.sync();
         hpf.zero();
         vcf1.zero();
         vcf2.zero();

         vca_env.on();
         vcf_env.on();
      }
   }

   void noteOff(uint8_t velocity_)
   {
      vca_env.off();
      vcf_env.off();
   }

   void tick(const NoEffect& effect_, unsigned n_) {}

   SIG::Signal sample(const NoEffect& effect_)
   {
      SIG::Signal vco_out = vco_wave == SQUARE ? vco_square(vco_tuning)
                                               : vco_ramp(vco_tuning);

      float vcf_final_freq = vcf_freq + vcf_env() * env_mod;
      vcf1.setFreq(vcf_final_freq);
      vcf2.setFreq(vcf_final_freq);

      SIG::Signal vcf_out = vcf2( vcf1( hpf( vco_out )));

      return volume( vca_env( vcf_out ));
   }

private:
   Wave       vco_wave{};
   SIG::Float vco_tuning{0.0};
   SIG::Float vcf_freq{};
   SIG::Float env_mod{};
   SIG::Float acc_mod{};

   SIG::Float accent{0.0};

   SIG::Osc::Square        vco_square{};
   SIG::Osc::Ramp          vco_ramp{};
   SIG::Filter::BiQuad     hpf{SIG::Filter::HIPASS};
   SIG::Filter::BiQuad     vcf1{SIG::Filter::LOPASS};
   SIG::Filter::FirstOrder vcf2{SIG::Filter::LOPASS};
   SIG::Env::Adsr          vcf_env{};
   SIG::Env::Adsr          vca_env{};
   SIG::Gain               volume{};
};

} // namespace TB_303
