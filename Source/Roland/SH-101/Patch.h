//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace SH_101 {

enum LfoWave : uint8_t { LFO_TRI = 0, LFO_SQR = 1, LFO_RAND = 2, LFO_NOISE = 3 };
enum VcoPwm  : uint8_t { PWM_MAN = 0, PWM_LFO = 1, PWM_ENV = 2 };
enum SubMode : uint8_t { SUB_2OS = 0, SUB_1OS = 1, SUB_2OP = 2 };
enum EnvMode : uint8_t { ENV_GPT = 0, ENV_LFO = 1, ENV_GTE = 2 };
enum VcaMode : uint8_t { VCA_GTE = 0, VCA_ENV = 1 };

struct Patch
{
   LfoWave lfo_wave{LFO_TRI};
   float   lfo_rate{0.0f};

   float   vco_mod{0.0f};
   int8_t  vco_range{0};         // 16' 8' 4' 2'
   float   vco_pulse_width{0.0f};
   VcoPwm  vco_pwm_src{PWM_MAN};

   float   source_square{9.9f};
   float   source_ramp{9.9f};
   float   source_sub{0.0f};
   SubMode source_sub_mode{SUB_1OS};
   float   source_noise{0.0f};

   float   vcf_freq{9.9f};
   float   vcf_res{0.0f};
   float   vcf_env{0.0f};
   float   vcf_mod{0.0f};
   float   vcf_kybd{0.0f};

   EnvMode env_mode{ENV_GTE};
   float   env_a{0.0f};
   float   env_d{0.0f};
   float   env_s{9.9f};
   float   env_r{0.0f};

   VcaMode vca_mode{VCA_GTE};

   const char* name{};
};

extern const unsigned NUM_PROGRAM;

extern const Patch program[];

} // namespace SH_101
