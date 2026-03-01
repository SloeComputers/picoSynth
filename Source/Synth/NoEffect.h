//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/SIG.h"

class NoEffect
{
public:
   NoEffect() = default;

   //! Configure the voice
   template <typename PATCH>
   void program(const PATCH* patch_) {}

   void tick(unsigned n_) {}

   //! Signal processing pre voice signal generation
   void pre() {}

   //! Signal processing post voice signal generation on the summed voices
   SIG::Signal post(SIG::Signal signal_) { return signal_; }
};
