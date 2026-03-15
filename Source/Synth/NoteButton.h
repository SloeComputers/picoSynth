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

   void init(uint8_t              note_up_,
             uint8_t              note_dn_,
             unsigned             n_,
             const Control::Enum* enum_,
             const char*          name_,
             uint8_t&             patch_,
             unsigned             width_ = 8,
             unsigned             lsb_   = 0)
   {
      note_up    = note_up_;
      note_dn    = note_dn_;
      n          = n_;
      enum_table = enum_;
      name       = name_;

      patch.init(patch_, width_ , lsb_);
   }

   bool edit(uint8_t note_, const char*& text_) const
   {
      if ((note_ != note_up) && (note_ != note_dn))
         return false;

#if 0
      uint8_t value = patch;

      if ((note_ == note_up) && (value < n))
      {
         ++value;
      }
      else if ((note_ == note_dn) && (value > 0))
      {
         --value;
      }

      patch  = value;
      text_  = enum_table[value];
#endif
      return true;
   }

private:
   uint8_t              note_up{};
   uint8_t              note_dn{};
   uint8_t              n{};
   const char*          name{};
   const Control::Enum* enum_table;
   PatchRef             patch{};
};

