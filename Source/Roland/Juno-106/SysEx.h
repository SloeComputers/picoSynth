//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace Juno106 {

class SysEx
{
public:
   SysEx() = default;

   uint8_t lfo_rate;
   uint8_t lfo_delay;
   uint8_t dco_lfo;
   uint8_t dco_pwm;
   uint8_t noise_level;
   uint8_t vcf_freq;
   uint8_t vcf_res;
   uint8_t vcf_env;
   uint8_t vcf_lfo;
   uint8_t vcf_kbd;
   uint8_t vca_level;
   uint8_t env_attack;
   uint8_t env_decay;
   uint8_t env_sustain;
   uint8_t env_release;
   uint8_t sub_osc_level;

   enum Range { RANGE_4 = 0b100, RANGE_8 = 0b010, RANGE_16 = 0b001 };

   union
   {
      struct
      {
         uint8_t range      : 3;
         uint8_t pwm        : 1;
         uint8_t saw        : 1;
         uint8_t chorus_off : 1;
         uint8_t chorus_1   : 1;
      };

      uint8_t btns1;
   };

   enum Hpf { HPF_0 = 0b11, HPF_1 = 0b10 , HPF_2 = 0b01, HPF_3 = 0b00 };

   union
   {
      struct
      {
         uint8_t dco_pwm_man : 1;
         uint8_t vca_gate    : 1;
         uint8_t vcf_neg     : 1;
         uint8_t hpf         : 2;
      };

      uint8_t btns2;
   };

   void print(const char* name_);
};

} // namespace Juno106
