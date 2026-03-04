//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

namespace SH_101 {

enum PortaMode : uint8_t { PORTA_ON = 0, PORTA_AUTO = 1, PORTA_OFF = 2 };

struct Control
{
   PortaMode porta_mode{};
   float     bend{};
   float     modulation{};
   float     tune{};
   float     volume{};
   float     portamento{};
   float     bend_vco{};
   float     bend_vcf{};
   float     lfo_mod{};

   // Model tuning controls
   float tune1{};
   float tune2{};
   float tune3{};
};

} // namespace SH_101
