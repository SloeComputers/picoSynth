//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "../SynthVoice.h"
#include "VL1/Voice.h"
#include "VL1/Program.h"
#include "VL1/Control.h"

namespace VL1 {

class Synth : public ::SynthVoice<Voice, /* NUM_VOICES */ 8>
{
public:
   Synth() = default;

private:
   void synthInit() override
   {
      setText(0, "   CASIO VL-1   ");

      control.init();
   }

   void voiceProgram(unsigned index_, uint8_t num_) override
   {
      if ((index_ != 0) || (num_ >= NUM_PROGRAM))
         return;

      patch = program[num_];

      patch.print();

      char text[32];
      snprintf(text, sizeof(text), "%s %u%u%u%u%u%u%u%u",
              patch.name, patch.wave, patch.attack_time, patch.decay_time, patch.sustain_level,
              patch.sustain_time, patch.release_time, patch.vibrato, patch.tremolo);
      setText(1, text);

      programVoices(&patch);
   }

   void voiceControl(unsigned index_, uint8_t control_, uint8_t value_) override
   {
      if (index_ != 0)
         return;

      switch(control_)
      {
      // Controls below map neatly to AKAI MIDImix and MPKmini
      case 19: case 2: patch.attack_time   = editInt("ATTACK",        value_,   0,  9); break;
      case 23: case 3: patch.decay_time    = editInt("DECAY",         value_,   0,  9); break;
      case 27: case 4: patch.sustain_level = editInt("SUSTAIN LEVEL", value_,   0,  9); break;
      case 31: case 5: patch.sustain_time  = editInt("SUSTAIN TIME",  value_,   0,  9); break;
      case 49: case 6: patch.release_time  = editInt("RELEASE",       value_,   0,  9); break;
      case 53: case 7: patch.vibrato       = editInt("VIBRATO",       value_,   0,  9); break;
      case 57: case 8: patch.tremolo       = editInt("TREMOLO",       value_,   0,  9); break;

      case 56:         control.tune        = editInt("TUNE",          value_, -50, 50); break;
      case 60: case 9: control.balance     = editInt("BALANCE",       value_, -50, 50); break;
      case 62:         control.volume      = editInt("VOLUME",        value_,   0, 99); break;
      }

      programVoices(&patch);
      controlVoices(&control);
   }

   bool filterNote(uint8_t midi_note_) override
   {
      static const char* wave_str[10]  = {"WAVE 0 Piano",
                                          "WAVE 1 Fantasy",
                                          "WAVE 2 Violin",
                                          "WAVE 3 Flute",
                                          "WAVE 4 Guitar 1",
                                          "WAVE 5 Guitar 2",
                                          "WAVE 6 Horn",
                                          "WAVE 7 Electro-1",
                                          "WAVE 8 Electro-2",
                                          "WAVE 9 Electro-3"};

      static const char* octave_str[3] = {"OCTAVE LOW", "OCTAVE MIDDLE", "OCTAVE HIGH"};

      // Notes below map neatly to AKAI MIDImix mute/rec buttons
      switch(midi_note_)
      {
      case 1: if (modInt(patch.wave, +1, 10, wave_str)) programVoices(&patch); return true;
      case 3: if (modInt(patch.wave, -1, 10, wave_str)) programVoices(&patch); return true;

      case 4: if (modInt(control.octave, +1, 3, octave_str)) controlVoices(&control); return true;
      case 6: if (modInt(control.octave, -1, 3, octave_str)) controlVoices(&control); return true;

      default:
         break;
      }

      return false;
   }

   Program patch{};
   Control control{};
};

} // namespace VL1
