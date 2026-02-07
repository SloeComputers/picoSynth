//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/SIG.h"

#include "Patch.h"

namespace TB_303 {

class Effect
{
public:
   Effect() = default;

   //! Configure the voice
   void program(const Patch* patch_)
   {
   }

   void tick(unsigned n_) {}

   //! Signal processing pre voice signal generation
   void pre()
   {
   }

   //! Signal processing post voice signal generation on the summed voices
   SIG::Signal post(SIG::Signal signal_)
   {
      return signal_;
   }
};

} // namespace TB_303
