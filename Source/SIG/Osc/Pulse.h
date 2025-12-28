//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Osc/Base.h"

namespace Osc {

class Pulse : public Base
{
public:
   Pulse() = default;

   template <typename TYPE>
   void setPattern(TYPE pattern_)
   {
      unsigned num_bits        = sizeof(TYPE) * 8;
      unsigned samples_per_bit = PATTERN_LENGTH / num_bits;

      unsigned index = 0;

      for(unsigned bit = 0; bit < num_bits; ++bit)
      {
         TYPE mask  = TYPE(1) << (num_bits - bit - 1);
         bool value = (pattern_ & mask) != 0;

         for(unsigned j = 0; j < samples_per_bit; ++j)
         {
            pattern[index++] = value ? +1.0 : -1.0;
         }
      }
   }

   Sample operator()()
   {
      unsigned index  = phase >> (sizeof(Phase) * 8 - LOG2_PATTERN_LENGTH);
      Sample   sample = pattern[index];

      phase += delta;

      return gain(sample);
   }

   Sample operator()(Sample mod_)
   {
      setDelta(modDelta(mod_));

      unsigned index  = phase >> (sizeof(Phase) * 8 - LOG2_PATTERN_LENGTH);
      Sample   sample = pattern[index];

      phase += delta;

      return gain(sample);
   }

   Gain gain{};

private:
   static const unsigned LOG2_PATTERN_LENGTH = 5;
   static const uint64_t PATTERN_LENGTH      = 1 << LOG2_PATTERN_LENGTH;

   Sample pattern[PATTERN_LENGTH];
};

} // namespace Osc
