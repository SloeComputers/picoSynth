//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SynthVoice.h"

#include "Effect.h"
#include "Voice.h"
#include "Patch.h"

namespace PhysM {

class Synth : public ::SynthVoice<Effect, Voice, /* NUM_VOICES */ 8>
{
public:
   Synth()
   {
      addCtrl<float>(19, 2,   1.0,     10.0, "Exc Frq",  "Hz", patch.exciter_freq);
      addCtrl<float>(23, 3,  -0.999,   -0.9, "Exc Len",  "",   patch.exciter_length);
      addCtrl<float>(27, 4, 100.0,  10000.0, "Exc Cut",  "Hz", patch.exciter_cutoff);
      addCtrl<float>(31, 5,   0.0,     10.0, "Loop Dly", "mS", patch.delay);
      addCtrl<float>(49, 6, 100.0,  10000.0, "Loop Cut", "Hz", patch.delay_cutoff);
      addCtrl<float>(53, 7,  -1.0,      1.0, "Loop FB",  "",   patch.feedback);
      addCtrl<float>(62, 9, -60.0,     20.0, "Volume",   "dB", patch.volume);
   }

private:
   void synthInit() override
   {
      setText(0, "   Phys-Model   ");
   }

   void synthProgram(uint8_t num_) override
   {
      programVoices(&patch);
   }

   void synthEdit() override
   {
      programVoices(&patch);
   }

   Patch patch{};
};

} // namespace PhysM
