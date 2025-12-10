//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "../SynthBase.h"
#include "Simple/Voice.h"

namespace Simple {

class Synth : public ::SynthBase<Voice, /* NUM_VOICES */ 16>
{
public:
   Synth() = default;

   const char* getName() const override { return "     simple     "; }
};

} // namespace Simple
