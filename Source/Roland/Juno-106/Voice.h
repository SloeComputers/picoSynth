//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SysEx.h"
#include "Effect.h"

#include "SIG/SIG.h"

#include "Table_freq7.h"

namespace Juno106 {

class Voice
{
public:
   Voice()
   {
      lfo_env.setAttack(LFO_ENV_ATTACK);
   }

   //! Program a voice from a SYSEX patch
   void program(const SysEx* patch_)
   {
      lfo_env.setDelay(scaleMidi(patch_->lfo_delay, 0.0, LFO_DELAY_MAX));

      switch(patch_->range)
      {
      case SysEx::RANGE_16: transpose = -12; break;
      case SysEx::RANGE_8:  transpose =   0; break;
      case SysEx::RANGE_4:  transpose = +12; break;
      default:              transpose =   0; break;
      }

      updateDCOFreq();

      if (patch_->dco_pwm_man)
      {
         dco_pwm.setWidth(scaleMidi(patch_->dco_pwm, 0.0, DCO_PWM_MAX));
         dco_pwm_lfo = false;
      }
      else
      {
         dco_pwm_lfo_gain = 0.5 * float(patch_->dco_pwm) / 127;
         dco_pwm_lfo      = true;
      }

      dco_lfo = float(patch_->dco_lfo) / 127;

      dco_saw.gain = patch_->saw ? 1.0 : 0.0;
      dco_pwm.gain = patch_->pwm ? 1.0 : 0.0;
      dco_sub.gain = SIG::dBAttenLookup_7(0x7F - patch_->sub_osc_level);
      noise.gain   = SIG::dBAttenLookup_7(0x7F - patch_->noise_level);

      vcf.setFreq(table_freq7[patch_->vcf_freq]);
      vcf.setQ(scaleMidi(patch_->vcf_res, 0.1, 4.0));

      env.setAttack_mS(1 + patch_->env_attack * 3000 / 127);
      env.setDecay_mS(1 + patch_->env_decay * 12000 / 127);
      env.setSustain(patch_->env_sustain);
      env.setRelease_mS(1 + patch_->env_release * 12000 / 127);

      vca      = SIG::dBGainLookup((patch_->vca_level * 40.0f / 127) - 20.0f);
      vca_gate = patch_->vca_gate;
   }

   void noteOn(uint8_t note_, uint8_t velocity_)
   {
      note = note_;

      updateDCOFreq();

      dco_saw.sync();
      dco_sub.sync();
      dco_pwm.sync();

      lfo_env.on();
      env.on();

      gate = 1.0;
   }

   void noteOff(uint8_t velocity_)
   {
      env.off();

      gate = 0.0;
   }

   void noteBend(int16_t bend_) {}

   void tick(const Effect& effect_, unsigned n_) {}

   SIG::Signal sample(const Effect& effect_)
   {
      SIG::Signal lfo_out = effect_.lfo_out * lfo_env();

      if (dco_pwm_lfo)
      {
         dco_pwm.setWidth((1.0 + lfo_out) * dco_pwm_lfo_gain);
      }

      SIG::Signal fmod = lfo_out * dco_lfo;

      SIG::Signal dco_out = (dco_saw(fmod) + dco_pwm(fmod) + dco_sub(fmod) + noise()) / 4;

      SIG::Signal env_out = env();

      return vca(vcf(dco_out) * (vca_gate ? gate : env_out));
   }

private:
   void updateDCOFreq()
   {
      dco_saw.setNote(transpose + note);
      dco_sub.setNote(transpose + note - 12);
      dco_pwm.setNote(transpose + note);
   }

   //! Scale a MIDI parameter 0..127 to a floating point value
   static float scaleMidi(uint8_t midi_, float min_, float max_)
   {
      return min_ + ((max_ - min_) * midi_ / 127);
   }

   static constexpr float DCO_PWM_MAX    = 0.95; //!< DCO PWM 10   => 95% duty cycle

   static constexpr float LFO_DELAY_MAX  = 3;    //!< LFO DELAY 10 => 3s
   static constexpr float LFO_ENV_ATTACK = 0.2;  //!< LFO envelope attack time 0.2s

   signed                  transpose{0};
   uint8_t                 note{};
   SIG::LfoEnv             lfo_env{};
   SIG::Signal             dco_lfo{};
   SIG::Signal             dco_pwm_lfo_gain{};
   bool                    dco_pwm_lfo{};
   SIG::Osc::Ramp          dco_saw{};
   SIG::Osc::Pwm           dco_pwm{};
   SIG::Osc::Pwm           dco_sub{};
   SIG::Osc::Noise         noise{};
   SIG::Env::Adsr          env{};
   SIG::Filter::DualBiQuad vcf{SIG::Filter::LOPASS};
   SIG::Gain               vca{};
   bool                    vca_gate{};
   SIG::Signal             gate{};
};

} // namespace Juno106
