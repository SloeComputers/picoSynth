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
#include "Additive/Synth.h"

class SynthRack
{
public:
   SynthRack()
   {
      add(sh_101);
      add(tb_303);
      add(juno_106);
      add(vl1);
      add(acorn_proton);
      add(additive);
      add(physm);
      add(simple);
   }

   Synth* get() const { return synth; }

   void select(unsigned index_)
   {
      if (index >= num_synth)
         return;

      index = index_;

      synth = rack[index];
      synth->init();
   }

   void next()
   {
      select((index + 1) % num_synth);
   }

   void add(Synth& synth_)
   {
      if (num_synth == MAX_SYNTH)
         return;

      rack[num_synth++] = &synth_;
   }

private:
   static const unsigned MAX_SYNTH = 16;

   TB_303::Synth      tb_303{};
   Juno106::Synth     juno_106{};
   SH_101::Synth      sh_101{};
   VL1::Synth         vl1{};
   AcornProton::Synth acorn_proton{};
   PhysM::Synth       physm{};
   Simple::Synth      simple{};
   Additive::Synth    additive{};

   unsigned index{};
   Synth*   synth{};

   unsigned num_synth{0};
   Synth*   rack[MAX_SYNTH] = {};
};
