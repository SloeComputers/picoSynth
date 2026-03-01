//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

namespace SH_101 {

struct Control
{
   float tune{};
   float volume{};
   float portamento{};
   float bend_vco{};
   float bend_vcf{};
   float lfo_mod{};

   // Model tuning controls
   float tune1;
   float tune2;
   float tune3;
};

} // namespace SH_101
