//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/SIG.h"

#include "Patch.h"

namespace Simple {

class Effect
{
public:
   Effect() = default;

   //! Configure the voice
   void program(const Patch* patch_)
   {
   }

   //! Signal processing pre voice signal generation
   void pre()
   {
   }

   //! Signal processing post voice signal generation on the summed voices
   Sample post(Sample sample_)
   {
      return sample_;
   }
};

} // namespace Simple
