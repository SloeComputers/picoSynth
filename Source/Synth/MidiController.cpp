//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MidiController.h"

namespace Akai {

namespace MIDImix {

const uint8_t map[38] =
{
   9, 4,
   TOP1, TOP2, TOP3, TOP4, TOP5, TOP6, TOP7, TOP8, NONE,
   MID1, MID2, MID3, MID4, MID5, MID6, MID7, MID8, NONE,
   BTM1, BTM2, BTM3, BTM4, BTM5, BTM6, BTM7, BTM8, NONE,
   LVL1, LVL2, LVL3, LVL4, LVL5, LVL6, LVL7, LVL8, MASTER
};

} // MIDImix

namespace MPKmini {

const uint8_t map[10] =
{
   4, 2,
   K1, K2, K3, K4,
   K5, K6, K7, K8
};

} // MPKmini

} // Akai
