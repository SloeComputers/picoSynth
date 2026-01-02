//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/SIG.h"

#include "VL1/Patch.h"
#include "VL1/Control.h"
#include "VL1/Effect.h"

namespace VL1 {

class Voice
{
public:
   Voice() = default;

   void init()
   {
      lpf.setFreq(1000);

      perc_env.setAttack_mS(0);
      perc_env.setSustain(0);
      perc_env.setRelease_mS(0);

      octave_osc.setFreq(4);
   }

   void program(const Patch* patch_)
   {
      voice_transpose = 0;
      octave_osc.gain = 0.0;

      switch(patch_->wave)
      {
      case 0: // Piano
         melody.setPattern(uint16_t(0b1111111111100000));
         break;

      case 1: // Fantasy
         melody.setPattern(uint16_t(0b1111000011110000));
         break;

      case 2: // Violin
         melody.setPattern(uint16_t(0b1111011101101010));
         break;

      case 3: // Flute
         melody.setPattern(uint16_t(0b1111111100000000));
         break;

      case 4: // Guitar 1
         melody.setPattern(uint16_t(0b1000000100000000));
         voice_transpose = -12;
         break;

      case 5: // Guitar 2
         melody.setPattern(uint16_t(0b1011110011000000));
         voice_transpose = -12;
         break;

      case 6: // English Horn
         melody.setPattern(uint16_t(0b1100000000000000));
         voice_transpose = -12;
         break;

      case 7: // Electro-Sound 1 (octave modulated Piano)
         melody.setPattern(uint16_t(0b1111111111100000));
         octave_osc.gain = 12.0;
         break;

      case 8: // Electro-Sound 2 (octave modulated Fantasy)
         melody.setPattern(uint16_t(0b1111000011110000));
         octave_osc.gain = 12.0;
         break;

      case 9: // Electro-Sound 3 (octave modulated Flute)
         melody.setPattern(uint16_t(0b1111011101101010));
         octave_osc.gain = 12.0;
         break;
      }

      env.setAttack_mS(5 + patch_->attack_time * 50);
      env.setDecay_mS(patch_->decay_time * 800);
      env.setSustain(patch_->sustain_level * 127 / 9);
      env.setSustain_mS(patch_->sustain_time * 250);
      env.setRelease_mS(patch_->release_time * 250);

      vibrato.setFreq(1 + patch_->vibrato * 1.5);
      vibrato.gain = patch_->vibrato > 0 ? 0.1 : 0.0;  // 10 cents

      tremolo_on = patch_->tremolo > 0;
      if (tremolo_on)
      {
         tremolo.setFreq(1 + patch_->tremolo * 1.5);
         tremolo.gain = 0.5;
      }
   }

   void program(const Control* control_)
   {
      melody_transpose = (signed(control_->octave) - 1) * 12 + control_->tune / 50.0f;

      volume.setAtten_dB(-control_->gain);

      float pan = (50 + control_->balance) / 100.0f;

      perc_osc.gain.setPan(pan);
      perc_noise.gain.setPan(pan);
      melody.gain.setPan(1.0 - pan);
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
         vibrato.sync();
         tremolo.sync();
         octave_osc.sync();
         melody.setNote(note_ + voice_transpose);
         melody.sync();
         env.on();
         mode = MELODY;
         break;
      }
   }

   void noteOff(uint8_t velocity_)
   {
      env.off();
   }

   Signal sample(const Effect& effect_)
   {
      Signal value;

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
            Signal freq_mod = melody_transpose + vibrato() + octave_osc();

            value = env() * lpf(melody(freq_mod));

            if (tremolo_on)
            {
               value = value * (0.5f + tremolo());
            }
         }
         break;
      }

      return volume(value);
   }

private:
   enum Mode { PERC_OSC, PERC_NOISE, MELODY };

   signed                   voice_transpose{0};
   Mode                     mode{};
   Osc::PwmShift<1.0f,0.0f> octave_osc{};
   Osc::Triangle            vibrato{};
   float                    melody_transpose{};
   Osc::Pulse               melody{};
   Filter::FirstOrder       lpf{Filter::LOPASS};
   Adsr                     env{};
   bool                     tremolo_on{false};
   Osc::Triangle            tremolo{};
   Adsr                     perc_env{};
   Osc::Pwm                 perc_osc{};
   Osc::Noise               perc_noise{};
   Gain                     volume{};
};

} // namespace VL1
