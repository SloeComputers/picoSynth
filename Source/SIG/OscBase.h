//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Sample.h"

#include "Table_delta14_7.h"

class OscBase
{
public:
   OscBase() = default;

   void mute()
   {
      phase = 0;
      delta = 0;
   }

   //! Reset phase to zero
   void sync()
   {
      phase = 0;
   }

   void setFreq(float freq_hz_)
   {
      float ratio = freq_hz_ / SAMPLE_RATE;

      delta = sample2phase(ratio * 2.0);
   }

   //! Set frequency for MIDI note value
   void setNote(uint8_t midi_note_)
   {
      midi_note = midi_note_;
      updateExpFreq();
   }

   //! Set frequency offset
   void setDetune(uint8_t value7_)
   {
      exp_freq_lo7 = value7_;
      updateExpFreq();
   }

protected:
   Phase phase{0}; //!< Phase     (x2pi) Q0.32
   Phase delta{0}; //!< Phase inc (x2pi) Q0.32

   static const Phase PHASE_QUARTER = 0x40000000;
   static const Phase PHASE_HALF    = PHASE_QUARTER * 2;

private:
   void updateExpFreq()
   {
      unsigned index = (midi_note << FREQ_FRAC_BITS) + exp_freq_lo7;
      delta = table_delta14_7[index];
   }

   static const unsigned FREQ_FRAC_BITS = 7;

   uint8_t midi_note{0};    //!< MIDI note
   int8_t  exp_freq_lo7{0};
};
