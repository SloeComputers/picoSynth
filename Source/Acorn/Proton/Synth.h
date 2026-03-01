//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SynthVoice.h"

#include "Effect.h"
#include "Voice.h"
#include "Patch.h"

namespace AcornProton {

class Synth : public ::SynthVoice<Effect, Voice, /* NUM_VOICES */ 8>
{
public:
   Synth()
   {
      configure("Acorn Proton");

      addCtrl<uint8_t>(16, 0,    1, 127, "ENV T",   "", patch.env_t);
      addCtrl<int8_t> (17, 0, -128, 127, "ENV PI1", "", patch.env_pi1);
      addCtrl<int8_t> (21, 0, -128, 127, "ENV PI2", "", patch.env_pi2);
      addCtrl<int8_t> (25, 0, -128, 127, "ENV PI3", "", patch.env_pi3);
      addCtrl<uint8_t>(18, 0,    0, 255, "ENV PN1", "", patch.env_pn1);
      addCtrl<uint8_t>(22, 0,    0, 255, "ENV PN2", "", patch.env_pn2);
      addCtrl<uint8_t>(26, 0,    0, 255, "ENV PN3", "", patch.env_pn3);
      addCtrl<int8_t> (19, 0, -127, 127, "ENV AA",  "", patch.env_aa);
      addCtrl<int8_t> (23, 0, -127, 127, "ENV AD",  "", patch.env_ad);
      addCtrl<int8_t> (27, 0, -127,   0, "ENV AS",  "", patch.env_as);
      addCtrl<int8_t> (31, 0, -127,   0, "ENV AR",  "", patch.env_ar);
      addCtrl<uint8_t>(49, 0,    0, 126, "ENV ALA", "", patch.env_ala);
      addCtrl<uint8_t>(53, 0,    0, 126, "ENV ALD", "", patch.env_ald);
   }

private:
   void synthEdit() override
   {
      programVoices(&patch);
   }

   Patch patch{};
};

} // namespace AcornProton
