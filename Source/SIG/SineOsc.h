//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include <cmath>

#include "Gain.h"
#include "OscBase.h"

class SineOsc : public OscBase
{
public:
   SineOsc() = default;

   Sample operator()(Sample mod_ = 0)
   {
      float theta = phase2sample(phase) * M_PI;

      phase += delta + sample2phase(mod_);

      return gain(sinf(theta));
   }

   Gain gain{};
};
