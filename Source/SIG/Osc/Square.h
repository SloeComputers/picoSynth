//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Osc/Base.h"
#include "SIG/Gain.h"

namespace Osc {

class Square : public Base
{
public:
   Square() = default;

   Sample operator()()
   {
      Sample sample = phase < PHASE_HALF ? +1.0f : -1.0f;

      float t = phase2float(phase);
      sample += polyBLEP(t);
      t = phase2float(phase - PHASE_HALF);
      sample -= polyBLEP(t);

      phase += delta;

      return gain(sample);
   }

   Sample operator()(Sample mod_)
   {
      setDelta(modDelta(mod_));

      Sample sample = phase < PHASE_HALF ? +1.0f : -1.0f;

      float t = phase2float(phase);
      sample += polyBLEP(t);
      t = phase2float(phase - PHASE_HALF);
      sample -= polyBLEP(t);

      phase += delta;

      return gain(sample);
   }

   Gain gain{};
};

} // namespace Osc
