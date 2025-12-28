//------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//------------------------------------------------------------------------------
//
// \brief Juno-106 factory presets
//

#pragma once

#include <cstdint>

namespace Juno106 {

struct Patch
{
   uint8_t     raw[18];
   const char* name;
};

extern const unsigned NUM_PRESET;

extern Patch program[129];

}
