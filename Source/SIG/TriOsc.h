//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "OscBase.h"
#include "Gain.h"

class TriOsc : public OscBase
{
public:
   TriOsc() = default;

   Sample operator()()
   {
      Phase phase_shift = phase + PHASE_QUARTER;

      Phase p = phase_shift >= PHASE_HALF ? -phase * 2
                                          : +phase * 2;

      Sample sample = phase2sample(p);

      phase += delta;

      return gain(sample);
   }

   Sample operator()(Sample mod_)
   {
      Phase phase_shift = phase + PHASE_QUARTER;

      Phase p = phase_shift >= PHASE_HALF ? -phase * 2
                                          : +phase * 2;

      Sample sample = phase2sample(p);

      phase += modDelta(mod_);

      return gain(sample);
   }

   Gain gain{};
};
