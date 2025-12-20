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

   //! Set frequency
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

   //! Set exponential frequency detune relative to MIDI note (fixed-point-7)
   void setDetune(int32_t detune_)
   {
      exp_freq_detune = detune_;
      updateExpFreq();
   }

   //! Set exponential frequency detune relatine to MIDI note
   void setDetune(float detune_)
   {
      exp_freq_detune = signed(detune_ * EXP_FREQ_SCALE);
      updateExpFreq();
   }

protected:
   Phase    phase{0}; //!< Phase     (x2pi) Q0.32
   Phase    delta{0}; //!< Phase inc (x2pi) Q0.32

   static const Phase PHASE_QUARTER = 0x40000000;
   static const Phase PHASE_HALF    = PHASE_QUARTER * 2;

   //! Calculate delta for a frequency modulation input
   uint32_t modDelta(Sample mod_)
   {
      return table_delta14_7[exp_freq + signed(EXP_FREQ_SCALE * mod_)];
   }

private:
   void updateExpFreq()
   {
      exp_freq = (midi_note << EXP_FREQ_FRAC_BITS) + exp_freq_detune;
      delta    = table_delta14_7[exp_freq];
   }

   static const unsigned EXP_FREQ_FRAC_BITS = 7;
   static const unsigned EXP_FREQ_SCALE     = 1 << EXP_FREQ_FRAC_BITS;

   uint32_t exp_freq;           //!< Exponential frequency where 69.0 equivalent to 440 Hz (fixed-point-7)
   int32_t  exp_freq_detune{0}; //!< Detune (fixed-point-7)
   uint8_t  midi_note{0};       //!< MIDI note
};
