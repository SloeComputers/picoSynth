//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "Juno106/SysEx.h"

#include "SIG/TriOsc.h"
#include "SIG/RampOsc.h"
#include "SIG/PulseOsc.h"
#include "SIG/SineOsc.h"
#include "SIG/Noise.h"
#include "SIG/LfoEnv.h"
#include "SIG/Adsr.h"
#include "SIG/Gain.h"
#include "SIG/BiQuadFilter.h"

#include "Table_amp7.h"
#include "Table_gain7.h"

namespace Juno106 {

class Voice
{
public:
   Voice()
   {
      lfo_env.setAttack(LFO_ENV_ATTACK);
      lfo.gain = 1.0;
   }

   //! Program a voice from a SYSEX patch
   void program(const SysEx* patch_)
   {
      lfo_env.setDelay(scaleMidi(patch_->lfo_delay, 0.0, LFO_DELAY_MAX));
      lfo.setFreq(scaleMidi(patch_->lfo_rate, LFO_FREQ_MIN, LFO_FREQ_MAX));

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

      dco_lfo = 0.001 * float(patch_->dco_lfo) / 127;

      dco_saw.gain = patch_->saw ? 1.0 : 0.0;
      dco_pwm.gain = patch_->pwm ? 1.0 : 0.0;
      dco_sub.gain = table_amp7[patch_->sub_osc_level];
      noise.gain   = table_amp7[patch_->noise_level];

      vcf.setFreq(scaleMidi(patch_->vcf_freq, 10.0, 20000.0));
      vcf.setQ(scaleMidi(patch_->vcf_res, 0.1, 4.0));

      env.setAttack_mS(1 + patch_->env_attack * 3000 / 127);
      env.setDecay_mS(1 + patch_->env_decay * 12000 / 127);
      env.setSustain(patch_->env_sustain);
      env.setRelease_mS(1 + patch_->env_release * 12000 / 127);

      vca      = table_gain7[patch_->vca_level];
      vca_gate = patch_->vca_gate;
   }

   void mute()
   {
      env.mute();
      gate = 0;
   }

   void noteOn(uint8_t note_, uint8_t velocity_)
   {
      note = note_;

      updateDCOFreq();

      lfo.sync();
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

   Sample sample()
   {
      Sample lfo_out = lfo() * lfo_env();

      if (dco_pwm_lfo)
      {
         dco_pwm.setWidth((1.0 + lfo_out) * dco_pwm_lfo_gain);
      }

      Sample fmod = lfo_out * dco_lfo;

      Sample dco_out = (dco_saw(fmod) + dco_pwm(fmod) + dco_sub(fmod) + noise()) / 4;

      Sample env_out = env();

      return vca(vcf(dco_out) * (vca_gate ? gate : env_out));
   }

   static Sample effect(Sample sample_)
   {
      return sample_;
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

   static constexpr float LFO_FREQ_MIN   = 0.1;  //!< LFO FREQ   0 => 0.1 Hz
   static constexpr float LFO_FREQ_MAX   = 30.0; //!< LFO FREQ  10 => 30 Hz
   static constexpr float LFO_DELAY_MAX  = 3;    //!< LFO DELAY 10 => 3s
   static constexpr float LFO_ENV_ATTACK = 0.2;  //!< LFO envelope attack time 0.2s

   signed         transpose{0};
   uint8_t        note{};
   TriOsc         lfo{};
   LfoEnv         lfo_env{};
   Sample         dco_lfo{};
   Sample         dco_pwm_lfo_gain{};
   bool           dco_pwm_lfo{};
   RampOsc        dco_saw{};
   PulseOsc       dco_pwm{};
   PulseOsc       dco_sub{};
   Noise          noise{};
   Adsr           env{};
   Filter::BiQuad vcf{Filter::LOPASS};
   Gain           vca{};
   bool           vca_gate{};
   Sample         gate{};
};

} // namespace Juno106
