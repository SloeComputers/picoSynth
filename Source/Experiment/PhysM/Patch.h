//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

namespace PhysM {

struct Patch
{
   float exciter_freq{2.0};
   float exciter_length{-0.99};
   float exciter_cutoff{2000.0};
   float delay{5.0};
   float delay_cutoff{1000.0};
   float feedback{-0.9};
   float volume{0.0};
};

} // namespace PhysM
