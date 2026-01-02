//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/SIG.h"

#include "VL1/Patch.h"
#include "VL1/Control.h"

namespace VL1 {

class Effect
{
public:
   Effect() = default;

   void program(const Patch* patch_)
   {
   }

   void program(const Control* control_)
   {
   }

   void pre()
   {
   }

   Signal post(Signal signal_)
   {
      return signal_;
   }
};

} // namespace VL1
