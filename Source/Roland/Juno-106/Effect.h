//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SysEx.h"

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
   void program(const SysEx* sysex_)
   {
      lfo.setFreq(scaleMidi(sysex_->lfo_rate, LFO_FREQ_MIN, LFO_FREQ_MAX));

      enable_lsf = sysex_->hpf  == SysEx::HPF_0;
      enable_hpf = (sysex_->hpf == SysEx::HPF_2) || (sysex_->hpf == SysEx::HPF_3);
      if (enable_hpf)
      {
          hpf.setFreq(sysex_->hpf == SysEx::HPF_2 ? 225.0f /* Hz */
                                                  : 700.0f /* Hz */);
      }
   }

   void tick(unsigned n_) {}

   void pre()
   {
      lfo_out = lfo();
   }

   SIG::Signal post(SIG::Signal signal_)
   {
      return enable_hpf ? hpf(signal_)
                        : signal_;
   }

   SIG::Signal lfo_out{};

private:
   //! Scale a MIDI parameter 0..127 to a floating point value
   static float scaleMidi(uint8_t midi_, float min_, float max_)
   {
      return min_ + ((max_ - min_) * midi_ / 127);
   }

   static constexpr float LFO_FREQ_MIN   = 0.1;  //!< LFO FREQ   0 => 0.1 Hz
   static constexpr float LFO_FREQ_MAX   = 30.0; //!< LFO FREQ  10 => 30 Hz

   SIG::Osc::Triangle   lfo{};
   bool                 enable_lsf{false};
   bool                 enable_hpf{false};
   SIG::Filter::OnePole hpf{SIG::Filter::HIPASS};
};

} // namespace Juno106
