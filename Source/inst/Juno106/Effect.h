//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "Juno106/SysEx.h"

#include "SIG/SIG.h"

namespace Juno106 {

class Effect
{
public:
   Effect()
   {
      lfo.gain = 1.0;
   }

   //! Program control from a SYSEX patch
   void program(const SysEx* patch_)
   {
      lfo.setFreq(scaleMidi(patch_->lfo_rate, LFO_FREQ_MIN, LFO_FREQ_MAX));
   }

   void pre()
   {
      lfo_out = lfo();
   }

   Signal post(Signal signal_)
   {
      return signal_;
   }

   Signal lfo_out{};

private:
   //! Scale a MIDI parameter 0..127 to a floating point value
   static float scaleMidi(uint8_t midi_, float min_, float max_)
   {
      return min_ + ((max_ - min_) * midi_ / 127);
   }

   static constexpr float LFO_FREQ_MIN   = 0.1;  //!< LFO FREQ   0 => 0.1 Hz
   static constexpr float LFO_FREQ_MAX   = 30.0; //!< LFO FREQ  10 => 30 Hz

   Osc::Triangle lfo{};
};

} // namespace Juno106
