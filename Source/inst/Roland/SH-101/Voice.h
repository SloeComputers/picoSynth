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
      noise.gain = 3.8f;
      noise_filter.setFreq(1800.0f);
      noise_filter.setQ(0.40f);
   }

   void program(const Patch* patch_)
   {
      // LFO
      lfo_wave = patch_->lfo_wave;
      float lfo_freq = 35.0f * powf(2.0f, 255.0f * (patch_->lfo_rate * 0.1f - 1.0f) / 21.0f);
      lfo_triangle.setFreq(lfo_freq);
      lfo_square.setFreq(lfo_freq);
      lfo_random.setFreq(lfo_freq);

      // VCO
      vco_octave  = patch_->vco_range;
      vco_pwm_src = patch_->vco_pwm_src;
      vco_mod     = patch_->vco_mod * 0.1f;
      pwm_level   = patch_->vco_pulse_width * 0.1f;

      // SOURCE
      vco_rect.gain = log_pot(patch_->source_square) * -0.3f;
      vco_ramp.gain = log_pot(patch_->source_ramp)   * +0.4f;
      vco_sub.gain  = log_pot(patch_->source_sub)    * -0.2f;
      noise_mix     = log_pot(patch_->source_noise)  * +0.7f;

      switch(patch_->source_sub_mode)
      {
      case SUB_2OS: vco_sub_octave = -2; vco_sub.setWidth(0.0f); break;
      case SUB_1OS: vco_sub_octave = -1; vco_sub.setWidth(0.0f); break;
      case SUB_2OP: vco_sub_octave = -2; vco_sub.setWidth(-0.5f); break;
      }

      updateFreq();

      // VCF
      vcf.setFreq(16000.0f * powf(2.0f, 255.0f * (patch_->vcf_freq * 0.1f - 1.0f) / 21.0f));
      vcf.setQ(0.4f + patch_->vcf_res * 1.5f);
      vcf_env_mod = patch_->vcf_env  * 0.1f;
      vcf_lfo_mod = patch_->vcf_mod  * 0.1f;
      vcf_kbd_mod = patch_->vcf_kybd * 0.1f;

      // ENV
      env_mode = patch_->env_mode;
      env.setAttack_mS(unsigned(patch_->env_a* 400));
      env.setDecay_mS(unsigned(patch_->env_d* 1000));
      env.setSustain(patch_->env_s * 0.1f);
      env.setRelease_mS(unsigned(patch_->env_r * 1000));

      // VCA
      vca_mode = patch_->vca_mode;
   }

   void program(const Control* control_)
   {
      volume = SIG::dBGainLookup(control_->volume);
   }

   void noteOn(uint8_t note_, uint8_t velocity_)
   {
      note = note_;

      updateFreq();

      lfo_triangle.sync();
      lfo_square.sync();

      vco_rect.sync();
      vco_ramp.setPhase(SIG::UPHASE_HALF);
      vco_sub.sync();

      gate = 1.0;
      if (env_mode != ENV_LFO)
         env.on();
   }

   void noteOff(uint8_t velocity_)
   {
      gate = 0.0;
      if (env_mode != ENV_LFO)
         env.off();
   }

   void tick(const NoEffect& effect_, unsigned n_) {}

   SIG::Signal sample(const NoEffect& effect_)
   {
      SIG::Signal noise_out   = noise_filter( noise());
      SIG::Signal env_out     = env();
      SIG::Signal lfo_tri_out = lfo_triangle();

      SIG::Signal lfo_out;
      switch(lfo_wave)
      {
      case LFO_TRI:   lfo_out = lfo_tri_out;  break;
      case LFO_SQR:   lfo_out = lfo_square(); break;
      case LFO_RAND:  lfo_out = lfo_random(); break;
      case LFO_NOISE: lfo_out = noise_out;    break;
      }

      SIG::Signal pwm_out;
      switch(vco_pwm_src)
      {
      case PWM_MAN: pwm_out = pwm_level; break;
      case PWM_LFO: pwm_out = pwm_level * lfo_tri_out; break;
      case PWM_ENV: pwm_out = pwm_level * env_out; break;
      }
      vco_rect.setWidth(pwm_out * 0.91);

      SIG::Signal pitch_mod = vco_mod * lfo_out;

      SIG::Signal source_mix = vco_rect(pitch_mod) +
                               vco_ramp(pitch_mod) +
                               vco_sub(pitch_mod) +
                               noise_mix(noise_out);

      SIG::Signal vcf_mod = vcf_lfo_mod * lfo_out +
                            vcf_env_mod * env_out +
                            vcf_kbd_mod * cv;

      (void) vcf_mod;

      vca = vca_mode == VCA_ENV ? env_out : gate;

      return volume( vca( vcf( source_mix )));
   }

private:
   void updateFreq()
   {
      unsigned eff_note = note + vco_octave * 12;

      cv = eff_note / 12.0f - 5.0f;

      vco_rect.setNote(eff_note);
      vco_ramp.setNote(eff_note);
      vco_sub.setNote(eff_note + vco_sub_octave * 12);
   }

   const SIG::LogPot log_pot{/* max x */ 9.99f, /* break point y */ 0.2f};

   SIG::Osc::Noise      noise{};
   SIG::Filter::BiQuad  noise_filter{SIG::Filter::LOPASS};

   SIG::Float gate{};
   SIG::Float cv{};

   // LFO
   LfoWave            lfo_wave{};
   SIG::Osc::Triangle lfo_triangle{};
   SIG::Osc::Square   lfo_square{};
   SIG::Osc::Random   lfo_random{};

   // VCO
   SIG::Float      vco_mod;
   VcoPwm          vco_pwm_src;
   SIG::Float      pwm_level;
   uint8_t         note;              // Currently playing MIDI note
   unsigned        vco_octave{};
   unsigned        vco_sub_octave{};
   SIG::Osc::Pwm   vco_rect{};
   SIG::Osc::Ramp  vco_ramp{};
   SIG::Osc::Pwm   vco_sub{};

   // SOURCE
   SIG::Gain       noise_mix;

   // VCF
   SIG::Signal         vcf_lfo_mod;
   SIG::Signal         vcf_env_mod;
   SIG::Signal         vcf_kbd_mod;
   SIG::Filter::BiQuad vcf{SIG::Filter::LOPASS};

   // ENV
   EnvMode        env_mode{};
   SIG::Env::Adsr env{};

   // VCA
   VcaMode   vca_mode{};
   SIG::Gain vca{};

   // AMP
   SIG::Gain volume{};
};

} // namespace SH_101
