//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/SIG.h"

#include "Patch.h"

namespace AcornProton {

class Effect
{
public:
   Effect() = default;

   //! Configure the voice
   void program(const Patch* patch_)
   {
   }

   //! Low frequency signal processing
   void tick(unsigned n_)
   {
   }

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

} // namespace AcornProton
