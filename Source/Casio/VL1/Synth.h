//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SynthVoice.h"

#include "Effect.h"
#include "Control.h"
#include "Patch.h"
#include "Voice.h"

namespace VL1 {

class Synth : public ::SynthVoice<Effect, Voice, /* NUM_VOICES */ 8>
{
public:
   Synth()
   {
      configure("CASIO VL-1");

      static const ::Control::Enum enm_wave[10] =
      {
          {0, "0 Piano"},
          {1, "1 Fantasy"},
          {2, "2 Violin"},
          {3, "3 Flute"},
          {4, "4 Guitar 1"},
          {5, "5 Guitar 2"},
          {6, "6 Horn"},
          {7, "7 Electro-1"},
          {8, "8 Electro-2"},
          {9, "9 Electro-3"}
      };

      addCtrl<uint8_t>(MX::LVL1, MK::K1, 10, enm_wave, "WAVE  ", patch.wave);

      addCtrl<uint8_t>(MX::LVL2, MK::K2, 0, 9, "ATTACK",        "", patch.attack_time);
      addCtrl<uint8_t>(MX::LVL3, MK::K3, 0, 9, "DECAY",         "", patch.decay_time);
      addCtrl<uint8_t>(MX::LVL4, MK::K4, 0, 9, "SUSTAIN LEVEL", "", patch.sustain_level);
      addCtrl<uint8_t>(MX::LVL5, MK::K5, 0, 9, "SUSTAIN TIME",  "", patch.sustain_time);
      addCtrl<uint8_t>(MX::LVL6, MK::K6, 0, 9, "RELEASE",       "", patch.release_time);
      addCtrl<uint8_t>(MX::LVL7, MK::K7, 0, 9, "VIBRATO",       "", patch.vibrato);
      addCtrl<uint8_t>(MX::LVL8, MK::K8, 0, 9, "TREMOLO",       "", patch.tremolo);

      static const ::Control::Enum enm_octave[3] =
         {{0, "LOW"}, {1, "MIDDLE"}, {2, "HIGH"}};

      addCtrl<uint8_t>(MX::BTM1, NONE, 3, enm_octave, "OCTAVE ", control.octave);
      addCtrl<signed>(  MX::BTM2,   NONE, -50,  +50, "TUNE",    "",   control.tune);
      addCtrl<signed>(  MX::BTM3,   NONE, -50,  +50, "BALANCE", "",   control.balance);
      addCtrl<unsigned>(MX::BTM4,   NONE,   0, +127, "DEBUG",   "",   control.debug);
      addCtrl<unsigned>(MX::MASTER, NONE, -30,    0, "VOLUME",  "dB", control.gain);
   }

private:
   void synthInit() override
   {
      control.init();

      initVoices();
   }

   void synthProgram(uint8_t num_) override
   {
      patch = program[num_];

      patch.print();

      char text[32];
      snprintf(text, sizeof(text), "%s %u%u%u%u%u%u%u%u",
              patch.name, patch.wave, patch.attack_time, patch.decay_time, patch.sustain_level,
              patch.sustain_time, patch.release_time, patch.vibrato, patch.tremolo);
      setText(1, text);

      setNumber(num_);

      programVoices(&patch);
   }

   void synthEdit() override
   {
      programVoices(&patch);
      programVoices(&control);
   }

   Patch   patch{};
   Control control{};
};

} // namespace VL1
