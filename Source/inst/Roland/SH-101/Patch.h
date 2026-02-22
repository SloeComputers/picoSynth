//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace SH_101 {

struct Patch
{
   uint8_t lfo_wave{0};          // TRI SQR RND NSE
   float   lfo_rate{0};

   int8_t  vco_range{0};         // 16' 8' 4' 2'
   uint8_t vco_pwm_src{0};       // LFO MAN ENV
   float   vco_mod{0};
   float   vco_pulse_width{0};

   uint8_t source_sub_mode{0};   // 1-OCT-SQ 2-OCT-SQ 2-OCT-PULSE
   float   source_square{0};
   float   source_ramp{1};
   float   source_sub{0};
   float   source_noise{0};

   float   vcf_freq{0};
   float   vcf_res{0};
   float   vcf_env{0};
   float   vcf_mod{0};
   float   vcf_kybd{0};

   uint8_t vca_mode{0};        // ENV GATE

   uint8_t env_mode{0};        // GATE+TRIG GATE LFO
   float   env_a{0};
   float   env_d{0};
   float   env_s{0};
   float   env_r{0};

   const char* name{};
};

extern const unsigned NUM_PROGRAM;

extern const Patch program[];

} // namespace SH_101
