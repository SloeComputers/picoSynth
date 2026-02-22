//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cmath>

#include "SIG/SIG.h"

#include "NoEffect.h"
#include "Patch.h"
#include "Control.h"

namespace SH_101 {

class Voice
{
public:
   Voice()
   {
      vcf.setFreq(5000);
   }

   void program(const Patch* patch_)
   {
      // LFO
      lfo_wave = patch_->lfo_wave;
      float lfo_freq = 30.0f * powf(2.0f, 255.0f * (patch_->lfo_rate * 0.1f - 1.0f) / 51.0f);
      lfo_triangle.setFreq(lfo_freq);
      lfo_square.setFreq(lfo_freq);
      lfo_random.setFreq(lfo_freq);

      // VCO
      vco_octave = patch_->vco_range;
      vco_rect.setWidth(patch_->vco_pulse_width * 0.092);

      // SOURCE MIXER
      vco_rect.gain = patch_->source_square * -0.03;
      vco_ramp.gain = patch_->source_ramp   * +0.04;
      vco_sub.gain  = patch_->source_sub    * -0.02;
      noise.gain    = patch_->source_noise  * +0.07;

      switch(patch_->source_sub_mode)
      {
      case 0: vco_sub_octave = -2; break;
      case 1: vco_sub_octave = -1; break;
      case 2: vco_sub_octave = -2; break;
      }

      updateFreq();

      // VCF
      vcf.setFreq(16000.0f * powf(2.0f, 255.0f * (patch_->vcf_freq * 0.1f - 1.0f) / 21.0f));
      vcf.setQ(0.5f + patch_->vcf_res);

      // ENV
      vca_is_gate = patch_->vca_mode == 0;
      env.setAttack_mS(unsigned(patch_->env_a* 400));
      env.setDecay_mS(unsigned(patch_->env_d* 1000));
      env.setSustain(patch_->env_s * 0.1f);
      env.setRelease_mS(unsigned(patch_->env_r * 1000));
   }

   void program(const Control* control_)
   {
      volume = SIG::dBGainLookup(control_->volume);
   }

   void noteOn(uint8_t note_, uint8_t velocity_)
   {
      note = note_;

      updateFreq();

      vco_rect.sync();
      vco_ramp.setPhase(SIG::UPHASE_HALF);
      vco_sub.sync();

      env.on();
      gate = 1.0;
   }

   void noteOff(uint8_t velocity_)
   {
      env.off();
      gate = 0.0;
   }

   void tick(const NoEffect& effect_, unsigned n_) {}

   SIG::Signal sample(const NoEffect& effect_)
   {
      SIG::Signal noise_out = noise();
      SIG::Signal env_out   = env();
      SIG::Signal lfo_out;

      switch(lfo_wave)
      {
      case 0: lfo_out = lfo_triangle(); break;
      case 1: lfo_out = lfo_square();   break;
      case 2: lfo_out = lfo_random();   break;
      case 3: lfo_out = noise_out;      break;
      }

      (void) lfo_out;

      SIG::Signal source_mix = vco_rect() +
                               vco_ramp() +
                               vco_sub() +
                               noise_out;

      vca = vca_is_gate ? gate : env_out;

      return volume( vca( vcf( source_mix )));
   }

private:
   void updateFreq()
   {
      vco_rect.setNote(note + vco_octave * 12);
      vco_ramp.setNote(note + vco_octave * 12);
      vco_sub.setNote(note + vco_sub_octave * 12);
   }

   uint8_t note; // Currently playing MIDI note
 
   // LFO
   uint8_t            lfo_wave{};
   SIG::Osc::Triangle lfo_triangle{};
   SIG::Osc::Square   lfo_square{};
   SIG::Osc::Random   lfo_random{};
   SIG::Osc::Noise    lfo_noise{};

   // VCO
   unsigned        vco_octave{0};
   unsigned        vco_sub_octave{1};
   SIG::Osc::Pwm   vco_rect{};
   SIG::Osc::Ramp  vco_ramp{};
   SIG::Osc::Pwm   vco_sub{};
   SIG::Osc::Noise noise{};

   // VCF
   SIG::Filter::BiQuad vcf{SIG::Filter::LOPASS};

   // ENV + VCA
   bool           vca_is_gate{false};
   SIG::Float     gate{};
   SIG::Env::Adsr env{};
   SIG::Gain      vca{};

   SIG::Gain         volume{};
};

} // namespace SH_101
