//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SynthVoice.h"

#include "NoEffect.h"
#include "Voice.h"
#include "Patch.h"
#include "Control.h"

namespace TB_303 {

class Synth : public ::SynthVoice<NoEffect, Voice, /* NUM_VOICES */ 1, SIG::Clip::Hard>
{
public:
   Synth()
   {
      configure("TB-303");

      static const ::Control::Enum enm_wave[2] =
         {{RAMP, "RAMP"}, {SQUARE, "SQUARE"}};

      addCtrl<Wave>(MX::BTM1, NONE, 2, enm_wave, "WAVE", patch.vco_wave);

      // The TB-303 voice controls
      addCtrl<float>(MX::LVL1, MK::K1, 0.0f, +9.99f, "CUT OFF FRQ", "", patch.vcf_freq);
      addCtrl<float>(MX::LVL2, MK::K2, 0.0f, +9.99f, "RESONANCE  ", "", patch.vcf_resonance);
      addCtrl<float>(MX::LVL3, MK::K3, 0.0f, +9.99f, "ENV MOD    ", "", patch.env_mod);
      addCtrl<float>(MX::LVL4, MK::K4, 0.0f, +9.99f, "DECAY      ", "", patch.env_decay);
      addCtrl<float>(MX::LVL5, MK::K5, 0.0f, +9.99f, "ACCENT     ", "", patch.accent);

      // TB-303 performance controls
      addCtrl<float>(MX::TOP1,   MK::K7, -1.0f,  +1.00f, "Tuning", "st", control.tuning);
      addCtrl<float>(MX::MASTER, MK::K8, -60.0f, +20.0f, "Volume", "dB", control.volume);

      // Controls for internal modelling
      addCtrl<float>(MX::MID1, NONE, 20.0f, 120.0f, "HPF f", "Hz", control.hpf_freq);
      addCtrl<float>(MX::MID2, NONE, 0.5f,  1.50f,  "HPF Q", "",   control.hpf_q);
   }

private:
   void synthEdit() override
   {
      programVoices(&patch);
      programVoices(&control);
   }

   void synthProgram(uint8_t num_) override
   {
      if (num_ >= NUM_PROGRAM)
         return;

      patch = program[num_];

      setText(1, patch.name);

      programVoices(&patch);
      programVoices(&control);
   }

   Patch   patch{};
   Control control{};
};

} // namespace TB_303
