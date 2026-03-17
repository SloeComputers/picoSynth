//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

namespace Additive {

struct Patch
{
   struct Harmonic
   {
      float level{1.0};
      float attack{0.0};
      float decay{1.0};
      float release{1.0};
   };

   static constexpr unsigned NUM_HARMONIC = 8;

   Harmonic harm[NUM_HARMONIC];
   float    volume{};
};

} // namespace Additive
