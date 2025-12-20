//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "OscBase.h"
#include "Gain.h"

class PosPwmOsc : public OscBase
{
public:
   PosPwmOsc() = default;

   //! Set pulse width 0.0 => square
   void setWidth(Sample width_)
   {
      limit = PHASE_HALF + sample2phase(width_);
   }

   Sample operator()()
   {
      Sample sample = phase >= limit ? 0.0f : +1.0f;

      phase += delta;

      return gain(sample);
   }

   Sample operator()(Sample mod_)
   {
      Sample sample = phase >= limit ? 0.0f : +1.0f;

      phase += modDelta(mod_);

      return gain(sample);
   }

   Gain gain{};

private:
   Phase limit{PHASE_HALF};
};

