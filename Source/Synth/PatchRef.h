//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

class PatchRef
{
public:
   PatchRef() = default;

   PatchRef(uint8_t& patch_,
            unsigned width_,
            unsigned lsb_)
   {
      init(patch_, width_, lsb_);
   }

   void init(uint8_t& patch_,
             unsigned width_,
             unsigned lsb_)
   {
      patch = &patch_;
      lsb   = lsb_;
      mask  = ((1 << width_) - 1) << lsb_;
   }

   operator unsigned() const
   {
      return (*patch & mask) >> lsb;
   }

   unsigned operator=(unsigned value_) const
   {
      *patch = (*patch & ~mask) | ((value_ << lsb) & mask);
      return value_;
   }

private:
   uint8_t* patch{};
   uint8_t  mask{};
   uint8_t  lsb{};
};

