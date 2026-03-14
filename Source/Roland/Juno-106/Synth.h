//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstring>

#include "SynthVoiceSysEx.h"
#include "MidiController.h"

#include "Effect.h"
#include "Patch.h"
#include "Control.h"
#include "Voice.h"
#include "SysEx.h"

namespace MX = Akai::MIDImix;
namespace MK = Akai::MPKmini;

namespace Juno106 {

class Synth : public ::SynthVoiceSysEx<Effect, Voice, /* NUM_VOICES */ 6, /* MAX_SYSEX_SIZE */ 3 + 18>
{
public:
   Synth()
      : SynthVoiceSysEx(MIDI_MANUF_ID)
   {
      configure("JUNO-106");

      // LFO
      addCtrl<uint8_t>(MX::TOP1, MK::K1, 0, 99, "LFO RATE    ", "1", patch.lfo_rate);
      addCtrl<uint8_t>(MX::TOP2, MK::K2, 0, 99, "LFO DELAY   ", "1", patch.lfo_delay);

      // DCO
      static const ::Control::Enum enm_dco_range[3] =
         {{SysEx::RANGE_16, "16'"}, {SysEx::RANGE_8, "8' "}, {SysEx::RANGE_4, "4' "}};
      static const ::Control::Enum enm_pwm_mode[2] =
         {{0, "LFO"}, {1, "MAN"}};
      static const ::Control::Enum enm_off_on[2] =
         {{0, "OFF"}, {1, "ON"}};

      addCtrl<uint8_t>(MX::MID1, NONE, 3, enm_dco_range, "DCO RANGE   ", patch.btns1, 3, 0);
      addCtrl<uint8_t>(MX::MID2, NONE, 0, 99, "DCO LFO     ", "1", patch.dco_lfo);
      addCtrl<uint8_t>(MX::MID3, NONE, 0, 99, "DCO PWM     ", "1", patch.dco_pwm);
      addCtrl<uint8_t>(MX::MID4, NONE, 2, enm_pwm_mode, "DCO PWM MODE", patch.btns2, 1, 0);
      addCtrl<uint8_t>(MX::MID5, NONE, 2, enm_off_on, "DCO PWM WAVE", patch.btns1, 1, 3);
      addCtrl<uint8_t>(MX::MID6, NONE, 2, enm_off_on, "DCO SAW WAVE", patch.btns1, 1, 4);
      addCtrl<uint8_t>(MX::MID7, NONE, 0, 99, "DCO SUB     ", "1", patch.sub_osc_level);
      addCtrl<uint8_t>(MX::MID8, NONE, 0, 99, "DCO NOISE   ", "1", patch.noise_level);

      // HPF
      static const ::Control::Enum enm_hpf_mode[4] =
         {{SysEx::HPF_0, "0"}, {SysEx::HPF_1, "1",}, {SysEx::HPF_2, "2"}, {SysEx::HPF_3, "3",}};

      addCtrl<uint8_t>(MX::BTM1, NONE, 4, enm_hpf_mode, "HPF MODE    ", patch.btns2, 2, 3);

      // VCF
      static const ::Control::Enum enm_vcf_env[2] =
         {{1, "-ve"}, {0, "+ve"}};

      addCtrl<uint8_t>(MX::BTM2, MK::K3, 0, 99, "VCF FREQ    ", "1", patch.vcf_freq);
      addCtrl<uint8_t>(MX::BTM3, MK::K4, 0, 99, "VCF RES     ", "1", patch.vcf_res);
      addCtrl<uint8_t>(MX::BTM4, NONE, 2, enm_vcf_env, "VCF ENV     ", patch.btns2, 1, 2);
      addCtrl<uint8_t>(MX::BTM5, NONE,   0, 99, "VCF ENV     ", "1", patch.vcf_env);
      addCtrl<uint8_t>(MX::BTM6, NONE,   0, 99, "VCF LFO     ", "1", patch.vcf_lfo);
      addCtrl<uint8_t>(MX::BTM7, NONE,   0, 99, "VCF KYBD    ", "1", patch.vcf_kbd);

      // VCA
      static const ::Control::Enum enm_vca_mode[2] =
         {{0, "ENV"}, {1, "GATE"}};

      addCtrl<uint8_t>(MX::TOP3, NONE, 2, enm_vca_mode, "VCA MODE    ", patch.btns2, 1, 1);
      addCtrl<uint8_t>(MX::TOP4, NONE, 0, 99, "VCA LEVEL   ", "1", patch.vca_level);

      // ENV
      addCtrl<uint8_t>(MX::LVL5, MK::K5, 0, 99, "ENV ATTACK  ", "1", patch.env_attack);
      addCtrl<uint8_t>(MX::LVL6, MK::K6, 0, 99, "ENV DECAY   ", "1", patch.env_decay);
      addCtrl<uint8_t>(MX::LVL7, MK::K7, 0, 99, "ENV SUSTAIN ", "1", patch.env_sustain);
      addCtrl<uint8_t>(MX::LVL8, MK::K8, 0, 99, "ENV RELEASE ", "1", patch.env_release);

      // CHORUS
      static const ::Control::Enum enm_chorus_mode[3] =
         {{0b01, "OFF"}, {0b10, "I"}, {0b11, "II"}};

      addCtrl<uint8_t>(MX::TOP5, NONE, 3, enm_chorus_mode, "CHORUS      ", patch.btns1, 2, 5);

      // CONTROL
      static const ::Control::Enum enm_porta_mode[2] =
         {{PORTA_OFF, "OFF"}, {PORTA_ON, "ON"}};

      addCtrl<float>(    MX::MASTER, NONE, -60.0f, +20.0f,     "VOLUME",       "dB", control.volume);
      addCtrl<float>(    MX::LVL1,   NONE, 0.0f, +9.99f,       "BEND DCO   ",  "",   control.bend_vco);
      addCtrl<float>(    MX::LVL2,   NONE, 0.0f, +9.99f,       "BEND VCF   ",  "",   control.bend_vcf);
      addCtrl<float>(    MX::LVL3,   NONE, 0.0f, +9.99f,       "BEND LFO   ",  "",   control.bend_lfo);
      addCtrl<PortaMode>(MX::TOP7,   NONE, 2, enm_porta_mode , "PORTA MODE ",        control.porta_mode);
      addCtrl<float>(    MX::TOP8,   NONE, 0.0f, +9.99f,       "PORTMENTO  ",  "",   control.portamento);
   }

private:
   static const uint8_t MIDI_MANUF_ID = 0x41; //!< Roland

   void setPatch(const uint8_t* raw_, const char* name_)
   {
      memcpy((uint8_t*)&patch, raw_, sizeof(SysEx));

      patch.print(name_);
      programVoices(&patch);
      setText(1, name_);
   }

   void synthEdit() override
   {
      programVoices(&patch);
   }

   void synthProgram(uint8_t num_) override
   {
      const Patch& prog = program[num_];
      setPatch(prog.raw, prog.name);
   }

   //! System exclusive message handling
   void processSysEx() override
   {
      uint8_t func = sysex_buffer[0];

      switch(func)
      {
      case 0x30:
         if (sysex_size == 3 + sizeof(SysEx))
         {
            uint8_t chan = sysex_buffer[1] + 1;
            uint8_t num  = sysex_buffer[2];

            (void)chan;

            if (num < 128)
            {
               memcpy(program[num + 1].raw, &sysex_buffer[3], sizeof(SysEx));

               setText(1, "SYSEX preset");
            }
         }
         break;

      case 0x31:
         if (sysex_size == 3 + sizeof(SysEx))
         {
            uint8_t chan = sysex_buffer[1] + 1;
            uint8_t prog = sysex_buffer[2];

            (void)chan;

            if (prog == 0)
            {
               setPatch(&sysex_buffer[3], "SYSEX patch");
            }
         }
         break;

      case 0x32:
         if (sysex_size == 4)
         {
            uint8_t chan  = sysex_buffer[1] + 1;
            uint8_t param = sysex_buffer[2];
            uint8_t value = sysex_buffer[3];

            (void)chan;

            if (param < sizeof(SysEx))
            {
               ((uint8_t*)&patch)[param] = value;

               programVoices(&patch);

               setText(1, "SYSEX edit");
            }
         }
         break;
      }
   }

   SysEx   patch{};
   Control control{};
};

} // namespace Juno106
