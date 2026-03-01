//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <type_traits>
#include <cstdint>
#include <cstdio>

#include "PatchRef.h"

//! Link a MIDI control to a patch value with linear scaling
class Control
{
public:
   struct Enum { int8_t value; const char* txt; };

   Control() = default;

   template <typename TYPE>
   void init(uint8_t     midi1_,
             uint8_t     midi2_,
             TYPE        min_,
             TYPE        max_,
             const char* name_,
             const char* unit_,
             const Enum* enum_,
             TYPE&       patch_,
             unsigned    width_ = 0,
             unsigned    lsb_   = 0)
   {
      if (std::is_floating_point_v<TYPE>)
      {
         f.min = min_;
         f.max = max_;

         type = FLOAT32;
      }
      else
      {
         i.min = min_;
         i.max = max_;

         bool is_signed = std::is_signed_v<TYPE>;

         switch(sizeof(TYPE))
         {
         case 1: type = is_signed ? INT8  : UINT8; break;
         case 2: type = is_signed ? INT16 : UINT16; break;
         case 4: type = is_signed ? INT32 : UINT32; break;
         }

         if ((type == UINT8) && (width_ != 0))
         {
            type  = UINT8_FIELD;
            flags = ((width_ & 0b111) << 3) | (lsb_ & 0b111);
         }
      }

      if (strcmp(unit_, "1") == 0)
      {
         flags |= FLAGS_ONE_DEC_PLACE;
         unit_ = "";
      }

      midi1 = midi1_;
      midi2 = midi2_;
      name  = name_;
      unit  = unit_;
      enm   = enum_;
      patch = &patch_;
   }

   bool edit(uint8_t midi_, uint8_t value_, char* text_, size_t text_len_) const
   {
      if ((midi_ != midi1) && (midi_ != midi2))
         return false;

      if (type == FLOAT32)
      {
         float       value     = f.min + value_ * (f.max - f.min) / 127.0f;
         const char* sign      = "";
         unsigned    print_val = 0;

         if (f.min < 0.0f)
         {
            sign = value < 0 ? "-"
                             : value > 0 ? "+"
                                         : " ";

            print_val = abs(signed(value * 100.0f));
         }
         else
         {
            print_val = unsigned(value * 100.0f);
         }

         unsigned units = print_val / 100;
         unsigned frac  = print_val % 100;

         snprintf(text_, text_len_, "%s %s%u.%02u %s", name, sign, units, frac, unit);

         *(float*)patch = value;
      }
      else
      {
         signed value;

         if (enm != nullptr)
         {
            unsigned index = (value_ * i.max + 64) / 128;
            snprintf(text_, text_len_, "%s %s", name, enm[index].txt);
            value = enm[index].value;
         }
         else
         {
            value = i.min + (value_ * (i.max - i.min) + 64) / 128;

            const char* sign      = "";
            unsigned    print_val = 0;

            if (i.min < 0)
            {
               sign = value < 0 ? "-"
                                : value > 0 ? "+"
                                            : " ";

               print_val = abs(value);
            }
            else
            {
               print_val = unsigned(value);
            }

            if (flags & FLAGS_ONE_DEC_PLACE)
            {
                unsigned units = print_val / 10;
                unsigned frac  = print_val % 10;
                snprintf(text_, text_len_, "%s %s%u.%u %s", name, sign, units, frac, unit);
            }
            else
            {
                snprintf(text_, text_len_, "%s %s%u %s", name, sign, print_val, unit);
            }
         }

         switch(type)
         {
         case INT8:   *(int8_t*)patch   = int8_t(value); break;
         case UINT8:  *(uint8_t*)patch  = uint8_t(value); break;
         case INT16:  *(int16_t*)patch  = int16_t(value); break;
         case UINT16: *(uint16_t*)patch = uint16_t(value); break;
         case INT32:  *(int32_t*)patch  = int32_t(value); break;
         case UINT32: *(uint32_t*)patch = uint32_t(value); break;

         case UINT8_FIELD:
            {
               unsigned width = (flags >> 3) & 0b111;
               unsigned lsb   = flags & 0b111;

               PatchRef(*(uint8_t*)patch, width, lsb) = value;
            }
            break;

         default: break;
         }
      }

      return true;
   }

   void print()
   {
      if (midi2 != 128)
         printf("%3u, %3u : %s\n", midi1, midi2, name);
      else
         printf("%3u      : %s\n", midi1, name);
   }

   const char* checkLabel(uint8_t midi, unsigned n) const
   {
      return midi == (n == 1 ? midi1 : midi2) ? name : nullptr;
   }

private:
   enum Type : uint8_t
   {
      NONE = 0,
      INT8,
      UINT8,
      INT16,
      UINT16,
      INT32,
      UINT32,
      FLOAT32,
      UINT8_FIELD
   };

   static const uint8_t FLAGS_ONE_DEC_PLACE = 1 << 7;

   Type    type{NONE};
   uint8_t flags{0};
   uint8_t midi1{};
   uint8_t midi2{};

   union
   {
      struct
      {
         float min;
         float max;

      } f;

      struct
      {
         signed min;
         signed max;

      } i;
   };

   const char* name{};
   const char* unit{""};
   const Enum* enm{nullptr};
   void*       patch{};
};

