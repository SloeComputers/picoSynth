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
      // Controls below map neatly to AKAI MIDImix rotary and slider knobs
      addCtrl<uint8_t>(16, 2, 0, 99, "LFO RATE",  "1", patch.lfo_rate);
      addCtrl<uint8_t>(20, 3, 0, 99, "LFO DELAY", "1", patch.lfo_delay);

      addCtrl<uint8_t>(19, 4, 0, 99, "DCO LFO",   "1", patch.dco_lfo);
      addCtrl<uint8_t>(23, 5, 0, 99, "DCO PWM",   "1", patch.dco_pwm);
      addCtrl<uint8_t>(27, 6, 0, 99, "DCO SUB",   "1", patch.sub_osc_level);
      addCtrl<uint8_t>(31, 7, 0, 99, "DCO NOISE", "1", patch.noise_level);

      //addCtrl<uint8_t>(18, 0, 0,  3, "HPF",       "1", patch.hpf);

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
   }

private:
   void synthInit() override
   {
      setText(0, "    JUNO-106    ");
   }

   static const uint8_t MIDI_MANUF_ID = 0x41; //!< Roland

   void setPatch(const uint8_t* raw_, const char* name_)
   {
      memcpy((uint8_t*)&patch, raw_, sizeof(SysEx));

      patch.print(name_);
      programVoices(&patch);
      setText(1, name_);
   }

   uint8_t edit(const char* text_, uint8_t value_)
   {
      setText(1, text_);
      return value_;
   }

   uint8_t editNum(const char* name_, uint8_t value_)
   {
      char text[17];
      snprintf(text, sizeof(text), "%s %u", name_, value_);
      return edit(text, value_);
   }

   unsigned editBool(const char* name_,
                     const char* text_true_,
                     const char* text_false_,
                     unsigned    value_)
   {
      char text[17];
      snprintf(text, sizeof(text), "%s %s", name_, value_ ? text_true_ : text_false_);
      setText(1, text);
      return value_;
   }

   //! Override to intercept bottom notes as buttons
   bool synthFilterNote(uint8_t midi_note_) override
   {
      // Notes below map neatly to AKAI MIDImix mute and rec/arm button
      switch(midi_note_)
      {
      case  1:
         switch(patch.range)
         {
         case 0b001: patch.range = edit("DCO RANGE 4'",  0b100); break;
         case 0b100: patch.range = edit("DCO RANGE 8'",  0b010); break;
         case 0b010: patch.range = edit("DCO RANGE 16'", 0b001); break;
         }
         break;

      case  4: patch.dco_pwm_man  = editBool("DCO PWM", "MAN",  "LFO", patch.dco_pwm_man ^ 1); break;
      case  7: patch.pwm          = editBool("DCO PWM", "ON",   "OFF", patch.pwm         ^ 1); break;
      case 10: patch.saw          = editBool("DCO SAW", "ON",   "OFF", patch.saw         ^ 1); break;

      case  3: patch.vcf_neg      = editBool("VCF ENV", "-VE",  "+VE", patch.vcf_neg     ^ 1); break;
      case  6: patch.vca_gate     = editBool("VCA",     "GATE", "ENV", patch.vca_gate    ^ 1); break;
      case  9: patch.chorus_off   = editBool("CHORUS",  "OFF",  "ON",  patch.chorus_off  ^ 1); break;

      case 12:
         patch.chorus_1   = editBool("CHORUS",  "I",    "II",  patch.chorus_1    ^ 1);
         patch.chorus_off = 0;
         break;

      default:
         return ::Synth::synthFilterNote(midi_note_);
      }

      programVoices(&patch);
      return true;
   }

   void synthEdit() override
   {
      programVoices(&patch);
   }

   void synthControl(uint8_t control_, uint8_t value_) override
   {
      switch(control_)
      {
      case 18: patch.hpf = editNum("HPF", (value_ >> 5)) ^ 0b11; programVoices(&patch); return;
      default: break;
      }

      ::Synth::synthControl(control_, value_);
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
