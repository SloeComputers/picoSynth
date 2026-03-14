//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

namespace Juno106 {

enum PortaMode : uint8_t { PORTA_ON = 0, PORTA_OFF = 2 };

struct Control
{
   PortaMode porta_mode{};
   float     volume{};
   float     portamento{};
   float     bend_vco{};
   float     bend_vcf{};
   float     bend_lfo{};

   // Model tuning controls
   float tune1{};
   float tune2{};
   float tune3{};
};

} // namespace Juno106
