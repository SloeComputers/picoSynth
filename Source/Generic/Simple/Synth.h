//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SynthVoice.h"

#include "NoEffect.h"
#include "Voice.h"
#include "Patch.h"

namespace Simple {

class Synth : public ::SynthVoice<NoEffect, Voice, /* NUM_VOICES */ 1>
{
public:
   Synth()
   {
      configure("simple");

      addCtrl<uint8_t>(19, 2,      0,   127,  "f-coarse", "", patch.f_coarse);
      addCtrl<float>  (23, 3, -10.0f, 10.0f,  "N",        "", patch.n);
      addCtrl<float>  (27, 4,   0.5f,   4.0f, "Drive",    "", patch.drive);
      addCtrl<float>  (62, 9,   0.5f,  3.0f,  "Gain",     "", patch.gain);

      static const char* clip_enum[5] = {"NO CLIP",
                                         "HARD CLIP",
                                         "POLY CLIP",
                                         "POLY5 CLIP",
                                         "TANH CLIP"};

      addBtn(1, 3, 4, clip_enum, patch.clip);
   }

private:
   void synthEdit() override
   {
      programVoices(&patch);
   }

   Patch patch{};
};

} // namespace Simple
