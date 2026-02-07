//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SynthVoice.h"

#include "Effect.h"
#include "Voice.h"
#include "Patch.h"

namespace TB_303 {

class Synth : public ::SynthVoice<Effect, Voice, /* NUM_VOICES */ 1>
{
public:
   Synth()
   {
      addCtrl<float>(19, 2, -5.0f, +5.00f, "TUNING    ",  "", patch.vco_tuning);
      addCtrl<float>(23, 3,  0.0f, +9.99f, "CUT OFF FRQ", "", patch.vcf_freq);
      addCtrl<float>(27, 4,  0.0f, +9.99f, "RESONANCE  ", "", patch.vcf_resonance);
      addCtrl<float>(31, 5,  0.0f, +9.99f, "ENV MOD    ", "", patch.env_mod);
      addCtrl<float>(49, 6,  0.0f, +9.99f, "DECAY      ", "", patch.env_decay);
      addCtrl<float>(53, 7,  0.0f, +9.99f, "ACCENT     ", "", patch.accent);

      static const char* vco_wave_enum[] = {"WAVEFORM   RAMP", "WAVEFORM  SQUARE"};

      addTgl(1, vco_wave_enum, patch.vco_wave);
   }

private:
   void synthInit() override
   {
      setText(0, "     TB-303     ");
   }

   void synthEdit() override
   {
      programVoices(&patch);
   }

   Patch patch{};
};

} // namespace TB_303
