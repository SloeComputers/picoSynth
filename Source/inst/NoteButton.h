//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <cstdio>

#include "PatchRef.h"

//! Link a MIDI not to a patch value as an up/dn button control
class NoteButton
{
public:
   NoteButton() = default;

   void init(uint8_t     note_up_,
             uint8_t     note_dn_,
             unsigned    max_,
             const char* enum_table_[],
             uint8_t&    patch_,
             unsigned    width_ = 8,
             unsigned    lsb_   = 0)
   {
      note_up    = note_up_;
      note_dn    = note_dn_;
      max        = max_;
      enum_table = enum_table_;

      patch.init(patch_, width_ , lsb_);
   }

   bool edit(uint8_t note_, const char*& text_) const
   {
      if ((note_ != note_up) && (note_ != note_dn))
         return false;

      uint8_t value = patch;

      if ((note_ == note_up) && (value < max))
      {
         ++value;
      }
      else if ((note_ == note_dn) && (value > 0))
      {
         --value;
      }

      patch  = value;
      text_  = enum_table[value];
      return true;
   }

private:
   uint8_t      note_up{};
   uint8_t      note_dn{};
   uint8_t      max{};
   const char** enum_table;
   PatchRef     patch{};
};

