//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

namespace AcornProton {

struct Patch
{
   bool    noise;
   uint8_t noise_mode;
   uint8_t env_t;
   int8_t  env_pi1;
   int8_t  env_pi2;
   int8_t  env_pi3;
   uint8_t env_pn1;
   uint8_t env_pn2;
   uint8_t env_pn3;
   int8_t  env_aa;
   int8_t  env_ad;
   int8_t  env_as;
   int8_t  env_ar;
   uint8_t env_ala;
   uint8_t env_ald;
};

} // namespace AcornProton
