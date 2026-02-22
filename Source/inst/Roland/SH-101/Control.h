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
};

} // namespace SH_101
