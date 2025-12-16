//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "STB/MIDIInstrument.h"

//! Synth external interface
class Synth : public MIDI::Instrument
{
public:
   Synth(unsigned num_voices_)
      : MIDI::Instrument(num_voices_)
   {
   }

   void init()
   {
      setText(0, "");
      setText(1, "");

      synthInit();

      for(unsigned i = 0; i < num_voices; ++i)
      {
         voiceOff(i, 0);
      }

      voiceProgram(0, 0);
   }

   //! Get stereo samples from synth
   virtual void getSamples(uint32_t* buffer, unsigned n) = 0;

   //! Get display text for the given line if it has been updated
   const char* getText(unsigned line_)
   {
      if (not text_update[line_])
         return nullptr;

      text_update[line_] = false;

      return &text[line_][0];
   }

protected:
   virtual void synthInit()
   {
   }

   //! Update text for the given line
   void setText(unsigned line_, const char* text_)
   {
      text_update[line_] = false;

      unsigned i = 0;

      for(; (i < MAX_TEXT_LEN) && (text_[i] != '\0'); ++i)
         text[line_][i] = text_[i];

      for(; i < MAX_TEXT_LEN; ++i)
         text[line_][i] = ' ';

      text[line_][i]     = '\0';
      text_update[line_] = true;
   }

   signed editInt(const char* name_, uint8_t midi_value_, signed min_, signed max_)
   {
      signed value = min_ + midi_value_ * (max_ - min_) / 127;

      char text[17];
      if (min_ < 0)
      {
         char sign = value < 0 ? '-' : value > 0 ? '+' : ' ';
         snprintf(text, sizeof(text), "%s %c%d", name_, sign, abs(value));
      }
      else
      {
         snprintf(text, sizeof(text), "%s %d", name_, value);
      }
      setText(1, text);

      return value;
   }

private:
   static const unsigned MAX_TEXT_LEN   = 16;
   static const unsigned MAX_TEXT_LINES = 2;

   char text[MAX_TEXT_LINES][MAX_TEXT_LEN + 1];
   bool text_update[MAX_TEXT_LINES] = {};
};
