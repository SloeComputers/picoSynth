//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "Synth.h"
#include "SIG/SIG.h"

template <typename EFFECT, typename VOICE, unsigned NUM_VOICES, typename CLIP = SIG::Clip::No>
class SynthVoice : public Synth
{
public:
   SynthVoice()
      : Synth(NUM_VOICES)
   {
   }

protected:
   void initVoices()
   {
      for(unsigned i = 0; i < NUM_VOICES; ++i)
      {
         voice[i].init();
      }
   }

   template <typename PATCH>
   void programVoices(const PATCH* patch_)
   {
      effect.program(patch_);

      for(unsigned i = 0; i < NUM_VOICES; ++i)
      {
         voice[i].program(patch_);
      }
   }

   EFFECT effect{};
   VOICE  voice[NUM_VOICES];
   CLIP   clip{};

private:
   int16_t sample()
   {
      effect.pre();

      SIG::Signal output = 0.0;

      for(unsigned i = 0; i < NUM_VOICES; ++i)
      {
         output += voice[i].sample(effect);
      }

      output = effect.post(output / NUM_VOICES);

      return int16_t(clip(output) * 0x7FFF);
   }

   void getSamples(uint32_t* buffer, unsigned n) override
   {
      effect.tick(n);

      for(unsigned i = 0; i < NUM_VOICES; ++i)
      {
         voice[i].tick(effect, n);
      }

      for(unsigned i = 0; i < n; ++i)
      {
         int16_t mono = sample();

         buffer[i] = (mono << 16) | (mono & 0xFFFF);
      }
   }

   void voicePitchBend(unsigned index_, int16_t value_) override
   {
      for(unsigned i = 0; i < NUM_VOICES; ++i)
      {
         voice[i].noteBend(value_);
      }
   }

   void voiceOn(unsigned index_, uint8_t midi_note_, uint8_t velocity_) override
   {
      if (not synthFilterNote(midi_note_))
      {
         voice[index_].noteOn(midi_note_, velocity_);
      }
   }

   void voiceOff(unsigned index_, uint8_t velocity_) override
   {
      voice[index_].noteOff(velocity_);
   }
};
