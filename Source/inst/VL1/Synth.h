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
      // Control below map neatly to AKAI sliders and knobs
      case 19: patch.attack_time   = editInt("ATTACK",        value_,   0,  9); break;
      case 23: patch.decay_time    = editInt("DECAY",         value_,   0,  9); break;
      case 27: patch.sustain_level = editInt("SUSTAIN LEVEL", value_,   0,  9); break;
      case 31: patch.sustain_time  = editInt("SUSTAIN TIME",  value_,   0,  9); break;
      case 49: patch.release_time  = editInt("RELEASE",       value_,   0,  9); break;
      case 53: patch.vibrato       = editInt("VIBRATO",       value_,   0,  9); break;
      case 57: patch.tremolo       = editInt("TREMOLO",       value_,   0,  9); break;

      case 56: control.tune        = editInt("TUNE",          value_, -50, 50); break;
      case 60: control.balance     = editInt("BALANCE",       value_, -50, 50); break;
      case 62: control.volume      = editInt("VOLUME",        value_,   0, 99); break;
      }

      programVoices(&patch);
      controlVoices(&control);
   }

   void modWave(signed delta_)
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

      patch.wave += delta_;
      setText(1, wave_str[patch.wave]);
      programVoices(&patch);
   }

   void modOctave(signed delta_)
   {
      static const char* octave_str[3] = {"OCTAVE LOW", "OCTAVE MIDDLE", "OCTAVE HIGH"};

      control.octave += delta_;
      setText(1, octave_str[1 + control.octave]);
      controlVoices(&control);
   }

   bool filterNote(uint8_t midi_note_) override
   {
      // Notes below map neatly to AKAI MIDImix mute/rec buttons
      switch(midi_note_)
      {
      case 1: if (patch.wave < 9) modWave(+1); return true;
      case 3: if (patch.wave > 0) modWave(-1); return true;

      case 4: if (control.octave < +1) modOctave(+1); return true;
      case 6: if (control.octave > -1) modOctave(-1); return true;;

      default:
         break;
      }

      return false;
   }

   Program patch{};
   Control control{};
};

} // namespace VL1
