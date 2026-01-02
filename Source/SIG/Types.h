//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include <cstdint>

#if defined(SIG_FP64)
using Signal = double;
using Phase  = double;
#else
using Signal = float;
using Phase  = float;
#endif

using UPhase = uint32_t;

#if defined(SIG_44_1KHz)
static const unsigned SAMPLE_RATE{44100};   //!< 44.1 kHz
#else
static const unsigned SAMPLE_RATE{48000};   //!< 48 kHz
#endif

//! Convert 32-bit unsigned phase to floating-point -1.0..1.0 (pi)
inline Signal uphase2signal(UPhase uphase_)
{
   return Signal(int32_t(uphase_)) / 0x80000000;
}

//! Convert floating point sample -1.0..1.0 to 32-bit unsigned phase
inline UPhase signal2uphase(Signal signal_)
{
   return UPhase(int32_t(signal_ * 0x7FFFFFFF));
}

//! Convert 32-bit unsigned phase to floating-point 0.0..1.0 (2pi)
inline Phase uphase2float(UPhase uphase_)
{
   return Phase(uphase_ >> 1) / 0x80000000;
}

