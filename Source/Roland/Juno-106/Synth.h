//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstring>

#include "SynthVoiceSysEx.h"

#include "Effect.h"
#include "Patch.h"
#include "Voice.h"
#include "SysEx.h"

namespace Juno106 {

class Synth : public ::SynthVoiceSysEx<Effect, Voice, /* NUM_VOICES */ 6, /* MAX_SYSEX_SIZE */ 3 + 18>
{
public:
   Synth()
      : SynthVoiceSysEx(MIDI_MANUF_ID)
   {
      configure("JUNO-106");

      // Controls below map neatly to AKAI MIDImix rotary and slider knobs
      addCtrl<uint8_t>(16, 2, 0, 99, "LFO RATE",  "1", patch.lfo_rate);
      addCtrl<uint8_t>(20, 3, 0, 99, "LFO DELAY", "1", patch.lfo_delay);

      addCtrl<uint8_t>(19, 4, 0, 99, "DCO LFO",   "1", patch.dco_lfo);
      addCtrl<uint8_t>(23, 5, 0, 99, "DCO PWM",   "1", patch.dco_pwm);
      addCtrl<uint8_t>(27, 6, 0, 99, "DCO SUB",   "1", patch.sub_osc_level);
      addCtrl<uint8_t>(31, 7, 0, 99, "DCO NOISE", "1", patch.noise_level);

      addCtrl<uint8_t>(22, 8, 0, 99, "VCF FREQ",  "1", patch.vcf_freq);
      addCtrl<uint8_t>(26, 9, 0, 99, "VCF RES",   "1", patch.vcf_res);
      addCtrl<uint8_t>(30, 0, 0, 99, "VCF ENV",   "1", patch.vcf_env);
      addCtrl<uint8_t>(48, 0, 0, 99, "VCF LFO",   "1", patch.vcf_lfo);
      addCtrl<uint8_t>(52, 0, 0, 99, "VCF KYBD",  "1", patch.vcf_kbd);

      addCtrl<uint8_t>(62, 0, 0, 99, "VCA LEVEL", "1", patch.vca_level);

      addCtrl<uint8_t>(49, 0, 0, 99, "ENV A",     "1", patch.env_attack);
      addCtrl<uint8_t>(53, 0, 0, 99, "ENV D",     "1", patch.env_decay);
      addCtrl<uint8_t>(57, 0, 0, 99, "ENV S",     "1", patch.env_sustain);
      addCtrl<uint8_t>(61, 0, 0, 99, "ENV R",     "1", patch.env_release);

         // case 0b001: patch.range = edit("DCO RANGE 4'",  0b100); break;
         // case 0b100: patch.range = edit("DCO RANGE 8'",  0b010); break;
         // case 0b010: patch.range = edit("DCO RANGE 16'", 0b001); break;

      static const char* range_enum[]   = {"RANGE 4'", "RANGE 8'", "RANGE 16'"};
      static const char* chorus_enum[]  = {"CHORUS OFF", "CHORUS I", "CHORUS II"};
      static const char* pwm_enum[]     = {"PWM OFF", "PWM ON"};
      static const char* saw_enum[]     = {"SAW OFF", "SAW ON"};
      static const char* dco_pwm_enum[] = {"PWM LFO", "PWM MAN"};
      static const char* vca_enum[]     = {"VCA ENV", "VCA GATE"};
      static const char* vcf_enum[]     = {"VCF +VE", "VCF -VE"};

      addBtn( 1, 1, 2, range_enum,   patch.btns1, 3, 0);
      addTgl( 7,       pwm_enum,     patch.btns1, 3);
      addTgl(10,       saw_enum,     patch.btns1, 4);
      addBtn( 9, 9, 2, chorus_enum,  patch.btns1, 2, 5);

      addTgl( 4,       dco_pwm_enum, patch.btns2, 0);
      addTgl( 3,       vca_enum,     patch.btns2, 1);
      addTgl( 6,       vcf_enum,     patch.btns2, 2);

      addCtrl<uint8_t>(18, 0, 0,  3, "HPF",       "", patch.btns2, 2, 3);
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

   SysEx patch{};
};

} // namespace Juno106
