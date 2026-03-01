//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Osc/Base.h"
#include "SIG/Gain.h"

namespace SN76489 {

class Tone : public SIG::Osc::Base
{
public:
   Tone() = default;

   void setClock(unsigned clk_freq_)
   {
   }

   //! Set 10-bit period
   void setPeriod(unsigned value_)
   {        
      period = value_;
   }
      
   //! Set MS 4 bits of oscillator period
   void setPeriodHi(uint8_t value6_) { setPeriod((period & 0x00F) | (value6_ << 4)); }
         
   //! Set LS 6 bits of oscillator period
   void setPeriodLo(uint8_t value4_) { setPeriod((period & 0x3F0) | value4_); }
         
   //! Set 4-bit attenuation (x2 dB)
   void setAtten(uint8_t value4_)
   {  
      // XXX assume full attenuation is a special case -inf dB not -30 dB
      static const int16_t atten_to_level[16] =
      {
         0x7FFF, 0x65AC, 0x50C3, 0x4026, 0x32F5, 0x287A, 0x2027, 0x198A,
         0x1449, 0x101D, 0x0CCD, 0x0A2B, 0x0813, 0x066A, 0x0518, 0x0000
      };
         
         level = level > 0 ? +atten_to_level[value4_]
                           : -atten_to_level[value4_];
   }  

   Signal operator()()
   {
      Signal signal = phase < UPHASE_HALF ? +1.0f : -1.0f;

      float t = uphase2float(phase);
      signal += polyBLEP(t);
      t = uphase2float(phase - UPHASE_HALF);
      signal -= polyBLEP(t);

      phase += delta;

      return gain(signal);
   }

   Signal operator()(Signal mod_)
   {
      setDelta(modDelta(mod_));

      Signal signal = phase < UPHASE_HALF ? +1.0f : -1.0f;

      float t = uphase2float(phase);
      signal += polyBLEP(t);
      t = uphase2float(phase - UPHASE_HALF);
      signal -= polyBLEP(t);

      phase += delta;

      return gain(signal);
   }

private:
   Gain gain{};
};

} // namespace SN76489
