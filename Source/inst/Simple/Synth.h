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
   Synth() = default;

private:
   void synthInit() override
   {
      setText(0, "     simple     ");
   }

   void synthControl(uint8_t control_, uint8_t value_) override
   {
      switch(control_)
      {
      case 19: case 2:
         patch.value = editInt("VALUE", value_, 0, 127);
         programVoices(&patch);
         break;
      }
   }

   Patch patch{};
};

} // namespace Simple
