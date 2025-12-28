//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace VL1 {

struct Patch
{
   uint8_t wave{0};
   uint8_t attack_time{0};
   uint8_t decay_time{0};
   uint8_t sustain_level{0};
   uint8_t sustain_time{0};
   uint8_t release_time{0};
   uint8_t vibrato{0};
   uint8_t tremolo{0};

   const char* name{""};

   void print() const;
};

static const unsigned NUM_PROGRAM = 6;

extern Patch program[NUM_PROGRAM];

} // namespace VL1
