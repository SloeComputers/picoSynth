//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include <cmath>

#include "Sample.h"
#include "Table_atten15.h"

class Gain
{
public:
   Gain(Sample value_ = 1.0) : value(value_) {}

   void setAtten_dB(float atten_)
   {
      signed index = 0x7FFE * atten_ / 60.0;

      if (index < 0)
         value = 1.0;
      else if (index > 0x7FFF)
         value = 0.0;
      else
         value = table_atten15[index];
   }

   //! Set gain using a sine panning function [0,1] => [0,1]
   void setPan(float pan_)
   {
      value = sinf(pan_ * float(M_PI) / 2.0f);
   }

   operator Sample() const { return value; }

   Sample operator=(Sample value_) { return value = value_; }

   Sample operator()(Sample in_) { return in_ * value; }

private:
   Sample value{1.0};
};
