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

      // Controls below map neatly to AKAI MIDImix and MPKmini
      addCtrl<uint8_t>(19, 2, 0, 9, "ATTACK",        "", patch.attack_time);
      addCtrl<uint8_t>(23, 3, 0, 9, "DECAY",         "", patch.decay_time);
      addCtrl<uint8_t>(27, 4, 0, 9, "SUSTAIN LEVEL", "", patch.sustain_level);
      addCtrl<uint8_t>(31, 5, 0, 9, "SUSTAIN TIME",  "", patch.sustain_time);
      addCtrl<uint8_t>(49, 6, 0, 9, "RELEASE",       "", patch.release_time);
      addCtrl<uint8_t>(53, 7, 0, 9, "VIBRATO",       "", patch.vibrato);
      addCtrl<uint8_t>(57, 8, 0, 9, "TREMOLO",       "", patch.tremolo);

      addCtrl<signed>(  56, 0, -50,  +50, "TUNE",    "",   control.tune);
      addCtrl<signed>(  60, 9, -50,  +50, "BALANCE", "",   control.balance);
      addCtrl<unsigned>(61, 0,   0, +127, "DEBUG",   "",   control.debug);
      addCtrl<unsigned>(62, 0, -30,    0, "VOLUME",  "dB", control.gain);

      static const char* wave_enum[10]  = {"WAVE 0 Piano",
                                           "WAVE 1 Fantasy",
                                           "WAVE 2 Violin",
                                           "WAVE 3 Flute",
                                           "WAVE 4 Guitar 1",
                                           "WAVE 5 Guitar 2",
                                           "WAVE 6 Horn",
                                           "WAVE 7 Electro-1",
                                           "WAVE 8 Electro-2",
                                           "WAVE 9 Electro-3"};

      static const char* octave_enum[3] = {"OCTAVE LOW",
                                           "OCTAVE MIDDLE",
                                           "OCTAVE HIGH"};

      // Notes below map neatly to AKAI MIDImix mute/rec buttons
      addBtn(1, 3, 9, wave_enum,   patch.wave);
      addBtn(4, 6, 2, octave_enum, control.octave);
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
