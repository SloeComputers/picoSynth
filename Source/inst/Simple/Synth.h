//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SynthVoice.h"

#include "Effect.h"
#include "Voice.h"
#include "Patch.h"

namespace Simple {

class Synth : public ::SynthVoice<Effect, Voice, /* NUM_VOICES */ 8>
{
public:
   Synth()
   {
      addCtrl<unsigned>(19, 2, 0, 127, "VALUE", "", patch.value);
   }

private:
   void synthInit() override
   {
      setText(0, "     simple     ");
   }

   void synthEdit() override
   {
      programVoices(&patch);
   }

   Patch patch{};
};

} // namespace Simple
