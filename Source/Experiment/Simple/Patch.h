//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

namespace Simple {

struct Patch
{
   uint8_t clip{};
   uint8_t f_coarse{69};
   float   n{0};
   float   drive{1.0};
   float   gain{1.0};
};

} // namespace Simple
