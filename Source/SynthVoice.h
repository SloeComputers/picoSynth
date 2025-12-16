//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "Synth.h"

template <typename VOICE, unsigned NUM_VOICES>
class SynthVoice : public Synth
{
public:
   SynthVoice()
      : Synth(NUM_VOICES)
   {
   }

protected:
   template <typename PATCH>
   void programVoices(const PATCH* patch_)
   {
      for(unsigned i = 0; i < NUM_VOICES; ++i)
      {
         voice[i].program(patch_);
      }
   }

   template <typename CONTROL>
   void controlVoices(const CONTROL* control_)
   {
      for(unsigned i = 0; i < NUM_VOICES; ++i)
      {
         voice[i].control(control_);
      }
   }

   VOICE voice[NUM_VOICES];

private:
   int16_t sample()
   {
      Sample output = 0.0;

      for(unsigned i = 0; i < NUM_VOICES; ++i)
      {
         output += voice[i].sample();
      }

      output = VOICE::effect(output / NUM_VOICES);

      return int16_t(output * 0x7FFF);
   }

   void getSamples(uint32_t* buffer, unsigned n) override
   {
      for(unsigned i = 0; i < n; ++i)
      {
         int16_t mono = sample();

         buffer[i] = (mono << 16) | (mono & 0xFFFF);
      }
   }

   virtual bool filterNote(uint8_t midi_note_)
   {
      return false;
   }

   void voiceOn(unsigned index_, uint8_t midi_note_, uint8_t velocity_) override
   {
      if (not filterNote(midi_note_))
      {
         voice[index_].noteOn(midi_note_, velocity_);
      }
   }

   void voiceOff(unsigned index_, uint8_t velocity_) override
   {
      voice[index_].noteOff(velocity_);
   }
};
