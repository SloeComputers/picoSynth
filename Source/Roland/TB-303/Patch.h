//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace TB_303 {

enum Wave : uint8_t { RAMP = 0, SQUARE = 1 };

struct Patch
{
   Wave  vco_wave{RAMP};
   float vcf_freq{0.0f};
   float vcf_resonance{0.0f};
   float env_mod{0.0f};
   float env_decay{0.0f};
   float accent{0.0f};

   const char* name{};
};

extern const unsigned NUM_PROGRAM;

extern const Patch program[];

} // namespace TB_303
