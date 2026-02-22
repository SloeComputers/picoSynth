//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "TB-303/Synth.h"
#include "Juno-106/Synth.h"
#include "SH-101/Synth.h"
#include "VL1/Synth.h"
#include "PhysM/Synth.h"
#include "Simple/Synth.h"
#include "Proton/Synth.h"

class SynthRack
{
public:
   SynthRack()
   {
      select(0);
   }

   Synth* get() const { return synth; }

   void select(unsigned index_)
   {
      if (index >= NUM_SYNTH)
         return;

      index = index_;

      switch(index)
      {
      case 0: synth = &tb_303;       break;
      case 1: synth = &sh_101;       break;
      case 2: synth = &juno_106;     break;
      case 3: synth = &vl1;          break;
      case 4: synth = &physm;        break;
      case 5: synth = &acorn_proton; break;
      case 6: synth = &simple;       break;
      }

      synth->init();
   }

   void next()
   {
      select((index + 1) % NUM_SYNTH);
   }

private:
   static const unsigned NUM_SYNTH = 7;

   TB_303::Synth      tb_303{};
   Juno106::Synth     juno_106{};
   SH_101::Synth      sh_101{};
   VL1::Synth         vl1{};
   PhysM::Synth       physm{};
   Simple::Synth      simple{};
   AcornProton::Synth acorn_proton{};

   unsigned index{};
   Synth*   synth{};
};
