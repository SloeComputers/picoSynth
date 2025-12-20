//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/PosPwmOsc.h"
#include "SIG/PwmOsc.h"
#include "SIG/PulseOsc.h"
#include "SIG/TriOsc.h"
#include "SIG/Noise.h"
#include "SIG/Adsr.h"
#include "SIG/Gain.h"

#include "VL1/Program.h"
#include "VL1/Control.h"

namespace VL1 {

class Voice
{
public:
   Voice()
   {
      lpf.setFreq(3000);
      lpf.setQ(0.1);

      perc_env.setAttack_mS(0);
      perc_env.setSustain(0);
      perc_env.setRelease_mS(0);

      octave_osc.setFreq(4);
   }

   void program(const Program* patch_)
   {
      voice_transpose = 0;
      octave_osc.gain = 0.0;

      switch(patch_->wave)
      {
      case 0: // Piano
         melody_osc.setPattern(uint16_t(0b1111111111100000));
         break;

      case 1: // Fantasy
         melody_osc.setPattern(uint16_t(0b1111000011110000));
         break;

      case 2: // Violin
         melody_osc.setPattern(uint16_t(0b1111011101101010));
         break;

      case 3: // Flute
         melody_osc.setPattern(uint16_t(0b1111111100000000));
         break;

      case 4: // Guitar 1
         melody_osc.setPattern(uint16_t(0b1000000100000000));
         voice_transpose = -12;
         break;

      case 5: // Guitar 2
         melody_osc.setPattern(uint16_t(0b1011110011000000));
         voice_transpose = -12;
         break;

      case 6: // English Horn
         melody_osc.setPattern(uint16_t(0b1100000000000000));
         voice_transpose = -12;
         break;

      case 7: // Electro-Sound 1 (octave modulated Piano)
         melody_osc.setPattern(uint16_t(0b1111111111100000));
         octave_osc.gain = 12.0;
         break;

      case 8: // Electro-Sound 2 (octave modulated Fantasy)
         melody_osc.setPattern(uint16_t(0b1111000011110000));
         octave_osc.gain = 12.0;
         break;

      case 9: // Electro-Sound 3 (octave modulated Flute)
         melody_osc.setPattern(uint16_t(0b1111111100000000));
         octave_osc.gain = 12.0;
         break;
      }

      melody_env.setAttack_mS(5 + patch_->attack_time * 100);
      melody_env.setDecay_mS(patch_->decay_time * 400);
      melody_env.setSustain(patch_->sustain_level * 127 / 9);
      melody_env.setSustain_mS(patch_->sustain_time * 400);
      melody_env.setRelease_mS(patch_->release_time * 400);

      vibrato_osc.setFreq(1 + patch_->vibrato * 1.5);
      vibrato_osc.gain = patch_->vibrato > 0 ? 0.1 : 0.0;  // 10 cents

      tremolo_on = patch_->tremolo > 0;
      if (tremolo_on)
      {
         tremolo_osc.setFreq(1 + patch_->tremolo * 1.5);
         tremolo_osc.gain = 0.5;
      }
   }

   void control(const Control* control_)
   {
      melody_transpose = (signed(control_->octave) - 1) * 12 + control_->tune / 50.0f;

      if (control_->volume == 0)
         gain = 0.0;
      else
         gain.setAtten_dB((99 - control_->volume) * 30.0f / 99);

      float pan = (50 + control_->balance) / 100.0f;

      perc_osc.gain.setPan(pan);
      perc_noise.gain.setPan(pan);
      melody_osc.gain.setPan(1.0 - pan);
   }

   void noteOn(uint8_t note_, uint8_t velocity_)
   {
      switch(note_)
      {
      case 48: // Po
         perc_osc.sync();
         perc_osc.setFreq(770);
         perc_env.setDecay_mS(60);
         perc_env.on();
         mode = PERC_OSC;
         break;

      case 49: // Pi
         perc_osc.sync();
         perc_osc.setFreq(1667);
         perc_env.setDecay_mS(40);
         perc_env.on();
         mode = PERC_OSC;
         break;

      case 50: // Sha
         perc_env.setDecay_mS(320);
         perc_env.on();
         mode = PERC_NOISE;
         break;

      default:
         vibrato_osc.sync();
         tremolo_osc.sync();
         octave_osc.sync();
         melody_osc.setNote(note_ + voice_transpose);
         melody_osc.sync();
         melody_env.on();
         mode = MELODY;
         break;
      }
   }

   void noteOff(uint8_t velocity_)
   {
      melody_env.off();
   }

   Sample sample()
   {
      Sample value;

      switch(mode)
      {
      case PERC_OSC:
         value = perc_env() * perc_osc();
         break;

      case PERC_NOISE:
         value = perc_env() * perc_noise();
         break;

      case MELODY:
         {
            Sample freq_mod = melody_transpose + vibrato_osc() + octave_osc();

            value = melody_env() * lpf(melody_osc(freq_mod));

            if (tremolo_on)
            {
               value = value * (0.5f + tremolo_osc());
            }
         }
         break;
      }

      return gain(value);
   }

   static Sample effect(Sample sample_)
   {
      return sample_;
   }

private:
   enum Mode { PERC_OSC, PERC_NOISE, MELODY };

   signed         voice_transpose{0};
   Mode           mode{};
   PosPwmOsc      octave_osc{};
   TriOsc         vibrato_osc{};
   PulseOsc       melody_osc{};
   Adsr           melody_env{};
   float          melody_transpose{};
   bool           tremolo_on{false};
   TriOsc         tremolo_osc{};
   Filter::BiQuad lpf{Filter::LOPASS};
   Adsr           perc_env{};
   PwmOsc         perc_osc{};
   Noise          perc_noise{};
   Gain           gain{};
};

} // namespace VL1
