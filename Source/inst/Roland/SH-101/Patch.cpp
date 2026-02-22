//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "Patch.h"

namespace SH_101 {

const unsigned NUM_PROGRAM = 6;

const Patch program[NUM_PROGRAM] =
{
#if 0
//   WAVE     CUT   RES   MOD   DCY   ACC
   { RAMP,    9.99, 0.00, 0.00, 0.00, 0.00, "Ramp"        },
   { SQUARE,  9.99, 0.00, 0.00, 0.00, 0.00, "Square"      },
   { SQUARE,  0.13, 9.99, 0.00, 0.51, 0.80, "Acid Square" },
   { RAMP,    5.00, 9.99, 9.99, 0.82, 0.62, "Clean Saw"   },
   { SQUARE,  0.27,  0.1, 0.79, 0.17, 0.59, "Square"      },
   { RAMP,    0.62, 0.90, 0.31, 0.70, 0.39, "Saw"         }
#endif
};

} // namespace SH_101
