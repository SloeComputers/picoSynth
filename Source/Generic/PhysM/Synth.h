//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SynthVoice.h"
#include "MidiController.h"

#include "NoEffect.h"
#include "Voice.h"
#include "Patch.h"

namespace MX = Akai::MIDImix;
namespace MK = Akai::MPKmini;

namespace PhysM {

class Synth : public ::SynthVoice<NoEffect, Voice, /* NUM_VOICES */ 1, SIG::Clip::Poly>
{
public:
   Synth()
   {
      configure("Phys-Model");

      addCtrl<float>(MX::LVL1,   MK::K1,   1.0,     10.0, "Exc Frq",  "Hz", patch.exciter_freq);
      addCtrl<float>(MX::LVL2,   MK::K2,  -0.999,   -0.9, "Exc Len",  "",   patch.exciter_length);
      addCtrl<float>(MX::LVL3,   MK::K3, 100.0,  10000.0, "Exc Cut",  "Hz", patch.exciter_cutoff);
      addCtrl<float>(MX::LVL4,   MK::K4,   0.0,     10.0, "Loop Dly", "mS", patch.delay);
      addCtrl<float>(MX::LVL5,   MK::K5, 100.0,  10000.0, "Loop Cut", "Hz", patch.delay_cutoff);
      addCtrl<float>(MX::LVL6,   MK::K6,  -1.0,      1.0, "Loop FB",  "",   patch.feedback);
      addCtrl<float>(MX::MASTER, MK::K8, -60.0,     20.0, "Volume",   "dB", patch.volume);
   }

private:
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
