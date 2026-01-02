//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Delay.h"
#include "SIG/Gain.h"

class FFComb
{
public:
   FFComb() = default;

   Signal operator()(Signal x_)
   {
      return x_ + alpha(delay(x_));
   }

   Gain alpha{};

private:
   Delay<1> delay;
};
