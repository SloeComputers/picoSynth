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
#include "Control.h"

namespace MX = Akai::MIDImix;
namespace MK = Akai::MPKmini;

namespace SH_101 {

class Synth : public ::SynthVoice<NoEffect, Voice, /* NUM_VOICES */ 1, SIG::Clip::Hard>
{
public:
   Synth()
   {
      configure("SH-101");

      // LFO
      static const ::Control::Enum enm_lfo_wave[4] =
         {{LFO_TRI, "TRI"}, {LFO_SQR, "SQR"}, {LFO_RAND, "RAND"}, {LFO_NOISE, "NOISE"}};

      addCtrl<LfoWave>(MX::TOP1, NONE, 4, enm_lfo_wave,"LFO WAVE  ",      patch.lfo_wave);
      addCtrl<float>(  MX::TOP2, MK::K5, 0.0f, +9.99f, "LFO RATE   ", "", patch.lfo_rate);

      // VCO
      static const ::Control::Enum enm_vco_range[6] =
         {{-1, "64'"}, {0, "32'"}, {1, "16'"}, {2, "8' "}, {3, "4' "}, {4, "2' "}};
      static const ::Control::Enum enm_vco_pwm_src[3] =
         {{PWM_MAN, "MAN"}, {PWM_LFO, "LFO"}, {PWM_ENV, "ENV"}};

      addCtrl<float>(  MX::LVL1, MK::K6, 0.0f, +9.99f,     "VCO MOD    ",   "", patch.vco_mod);
      addCtrl<int8_t>( MX::BTM1, NONE, 6, enm_vco_range,   "VCO RANGE   ",      patch.vco_range);
      addCtrl<float>(  MX::LVL2, MK::K7, 0.0f, +9.99f,     "VCO P.WIDTH",   "", patch.vco_pulse_width);
      addCtrl<VcoPwm>( MX::LVL3, NONE, 3, enm_vco_pwm_src, "VCO PWM SRC ",      patch.vco_pwm_src);

      // SOURCE MIXER
      static const ::Control::Enum enm_sub_mode[3] =
         {{0, "2OS"}, {1, "1OS"}, {2, "2OP"}};

      addCtrl<float>(  MX::LVL4, MK::K1, 0.0f, +9.99f,  "SRC SQUARE ",    "", patch.source_square);
      addCtrl<float>(  MX::LVL5, MK::K2, 0.0f, +9.99f,  "SRC RAMP   ",    "", patch.source_ramp);
      addCtrl<float>(  MX::LVL6, MK::K3, 0.0f, +9.99f,  "SRC SUB OSC",    "", patch.source_sub);
      addCtrl<SubMode>(MX::LVL7, NONE, 3, enm_sub_mode, "SRC SUB MODE",       patch.source_sub_mode);
      addCtrl<float>(  MX::LVL8, MK::K4, 0.0f, +9.99f,  "SRC NOISE  ",    "", patch.source_noise);

      // VCF
      addCtrl<float>(  MX::MID4, NONE,   0.0f, +9.99f, "VCF FREQ   ", "", patch.vcf_freq);
      addCtrl<float>(  MX::MID5, NONE,   0.0f, +9.99f, "VCF RES    ", "", patch.vcf_res);
      addCtrl<float>(  MX::MID6, NONE,   0.0f, +9.99f, "VCF ENV    ", "", patch.vcf_env);
      addCtrl<float>(  MX::MID7, NONE,   0.0f, +9.99f, "VCF MOD    ", "", patch.vcf_mod);
      addCtrl<float>(  MX::MID8, NONE,   0.0f, +9.99f, "VCF KYBD   ", "", patch.vcf_kybd);

      // ENV
      static const ::Control::Enum enm_env_mode[3] =
         {{0, "G+T"}, {1, "LFO"}, {2, "GATE"}};

      addCtrl<EnvMode>(MX::BTM4, NONE, 3, enm_env_mode, "ENV MODE   ",     patch.env_mode);
      addCtrl<float>(  MX::BTM5, NONE,   0.0f, +9.99f,  "ENV ATTACK ", "", patch.env_a);
      addCtrl<float>(  MX::BTM6, NONE,   0.0f, +9.99f,  "ENV DECAY  ", "", patch.env_d);
      addCtrl<float>(  MX::BTM7, NONE,   0.0f, +9.99f,  "ENV SUSTAIN", "", patch.env_s);
      addCtrl<float>(  MX::BTM8, NONE,   0.0f, +9.99f,  "ENV RELEASE", "", patch.env_r);

      // VCA
      static const ::Control::Enum enm_vca_mode[2] =
         {{0, "GATE"}, {1, "ENV"}};

      addCtrl<VcaMode>(MX::BTM3, NONE, 2, enm_vca_mode, "VCA MODE   ", patch.vca_mode);

      // CONTROL
      addCtrl<float>(  MX::BTM2, NONE,      -5.0f,  +5.0f, "TUNE       ",  "", control.tune);
      addCtrl<float>(  MX::MASTER, MK::K8, -60.0f, +20.0f, "VOLUME", "dB",   control.volume);
      addCtrl<float>(  MX::TOP5, NONE,       0.0f, +9.99f, "PORTMENTO  ",  "", control.portamento);
      addCtrl<float>(  MX::TOP6, NONE,       0.0f, +9.99f, "BEND VCO   ",  "", control.bend_vco);
      addCtrl<float>(  MX::TOP7, NONE,       0.0f, +9.99f, "BEND VCF   ",  "", control.bend_vcf);
      addCtrl<float>(  MX::TOP8, NONE,       0.0f, +9.99f, "LFO MOD    ",   "", control.lfo_mod);

      // Model tuning and debug
      addCtrl<float>(MX::MID1, NONE, 0.0f, +1.0f,  "int 1      ", "", control.tune1);
      addCtrl<float>(MX::MID2, NONE, 0.0f, +1.0f,  "int 2      ", "", control.tune2);
      addCtrl<float>(MX::MID3, NONE, 0.0f, +1.0f,  "int 3      ", "", control.tune3);
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
   }

   Patch   patch{};
   Control control{};
};

} // namespace SH_101
