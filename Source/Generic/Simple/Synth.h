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

namespace Simple {

class Synth : public ::SynthVoice<NoEffect, Voice, /* NUM_VOICES */ 1>
{
public:
   Synth()
   {
      configure("simple");

      static const ::Control::Enum enm_clip_mode[5] =
      {{0, "NONE"}, {1, "HARD"}, {2, "POLY"}, {3, "POLY5"}, {4, "TANH"}};

      addCtrl<uint8_t>(MX::LVL1, MK::K1,      0,   127,    "f-coarse", "", patch.f_coarse);
      addCtrl<float>  (MX::LVL2, MK::K2, -10.0f, 10.0f,    "N",        "", patch.n);
      addCtrl<float>  (MX::LVL3, MK::K3,   0.5f,   4.0f,   "Drive",    "", patch.drive);
      addCtrl<float>  (MX::LVL4, MK::K4,   0.5f,  3.0f,    "Gain",     "", patch.gain);
      addCtrl<uint8_t>(MX::LVL5, MK::K5, 5, enm_clip_mode, "CLIP  ",       patch.clip);
   }

private:
   void synthEdit() override
   {
      programVoices(&patch);
   }

   Patch patch{};
};

} // namespace Simple
