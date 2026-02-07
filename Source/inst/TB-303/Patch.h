//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

namespace TB_303 {

struct Patch
{
   uint8_t vco_wave{0};
   float   vco_tuning{0};
   float   vcf_freq{0};
   float   vcf_resonance{0};
   float   env_mod{0};
   float   env_decay{0};
   float   accent{0};
};

} // namespace TB_303
