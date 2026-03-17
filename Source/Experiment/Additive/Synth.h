//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SynthVoice.h"

#include "NoEffect.h"
#include "Voice.h"
#include "Patch.h"

namespace Additive {

class Synth : public ::SynthVoice<NoEffect, Voice, /* NUM_VOICES */ 3>
{
public:
   Synth()
   {
      configure("Additive");

      addCtrl<float>(MX::LVL1, MK::K1, 0.0f, 1.00f, "1st Level  ", "", patch.harm[0].level);
      addCtrl<float>(MX::LVL2, MK::K2, 0.0f, 1.00f, "2nd Level  ", "", patch.harm[1].level);
      addCtrl<float>(MX::LVL3, MK::K3, 0.0f, 1.00f, "3rd Level  ", "", patch.harm[2].level);
      addCtrl<float>(MX::LVL4, MK::K4, 0.0f, 1.00f, "4th Level  ", "", patch.harm[3].level);
      addCtrl<float>(MX::LVL5, MK::K5, 0.0f, 1.00f, "5th Level  ", "", patch.harm[4].level);
      addCtrl<float>(MX::LVL6, MK::K6, 0.0f, 1.00f, "6th Level  ", "", patch.harm[5].level);
      addCtrl<float>(MX::LVL7, MK::K7, 0.0f, 1.00f, "7th Level  ", "", patch.harm[6].level);
      addCtrl<float>(MX::LVL8, MK::K8, 0.0f, 1.00f, "8th Level  ", "", patch.harm[7].level);

      addCtrl<float>(MX::TOP1, NONE,   0.0f, 1.00f, "1st Attack ", "", patch.harm[0].attack);
      addCtrl<float>(MX::TOP2, NONE,   0.0f, 1.00f, "2nd Attack ", "", patch.harm[1].attack);
      addCtrl<float>(MX::TOP3, NONE,   0.0f, 1.00f, "3rd Attack ", "", patch.harm[2].attack);
      addCtrl<float>(MX::TOP4, NONE,   0.0f, 1.00f, "4th Attack ", "", patch.harm[3].attack);
      addCtrl<float>(MX::TOP5, NONE,   0.0f, 1.00f, "5th Attack ", "", patch.harm[4].attack);
      addCtrl<float>(MX::TOP6, NONE,   0.0f, 1.00f, "6th Attack ", "", patch.harm[5].attack);
      addCtrl<float>(MX::TOP7, NONE,   0.0f, 1.00f, "7th Attack ", "", patch.harm[6].attack);
      addCtrl<float>(MX::TOP8, NONE,   0.0f, 1.00f, "8th Attack ", "", patch.harm[7].attack);

      addCtrl<float>(MX::MID1, NONE,   0.0f, 1.00f, "1st Decay  ", "", patch.harm[0].decay);
      addCtrl<float>(MX::MID2, NONE,   0.0f, 1.00f, "2nd Decay  ", "", patch.harm[1].decay);
      addCtrl<float>(MX::MID3, NONE,   0.0f, 1.00f, "3rd Decay  ", "", patch.harm[2].decay);
      addCtrl<float>(MX::MID4, NONE,   0.0f, 1.00f, "4th Decay  ", "", patch.harm[3].decay);
      addCtrl<float>(MX::MID5, NONE,   0.0f, 1.00f, "5th Decay  ", "", patch.harm[4].decay);
      addCtrl<float>(MX::MID6, NONE,   0.0f, 1.00f, "6th Decay  ", "", patch.harm[5].decay);
      addCtrl<float>(MX::MID7, NONE,   0.0f, 1.00f, "7th Decay  ", "", patch.harm[6].decay);
      addCtrl<float>(MX::MID8, NONE,   0.0f, 1.00f, "8th Decay  ", "", patch.harm[7].decay);

      addCtrl<float>(MX::BTM1, NONE,   0.0f, 1.00f, "1st Release", "", patch.harm[0].release);
      addCtrl<float>(MX::BTM2, NONE,   0.0f, 1.00f, "2nd Release", "", patch.harm[1].release);
      addCtrl<float>(MX::BTM3, NONE,   0.0f, 1.00f, "3rd Release", "", patch.harm[2].release);
      addCtrl<float>(MX::BTM4, NONE,   0.0f, 1.00f, "4th Release", "", patch.harm[3].release);
      addCtrl<float>(MX::BTM5, NONE,   0.0f, 1.00f, "5th Release", "", patch.harm[4].release);
      addCtrl<float>(MX::BTM6, NONE,   0.0f, 1.00f, "6th Release", "", patch.harm[5].release);
      addCtrl<float>(MX::BTM7, NONE,   0.0f, 1.00f, "7th Release", "", patch.harm[6].release);
      addCtrl<float>(MX::BTM8, NONE,   0.0f, 1.00f, "8th Release", "", patch.harm[7].release);

      addCtrl<float>(MX::MASTER, NONE, -60.0f, +20.0f, "VOLUME", "dB", patch.volume);
   }

private:
   void synthEdit() override
   {
      programVoices(&patch);
   }

   Patch patch{};
};

} // namespace Additive
