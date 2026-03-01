//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>
#include <cstdarg>

#include "SysEx.h"

// Helper for pretty printing
static void pretf(const char* fix, const char* format = "", ...)
{
   static const char* FG_GREEN   = "\033[32m";
   static const char* FG_DEFAULT = "\033[39m";

   printf("%s%s%s", FG_GREEN, fix, FG_DEFAULT);

   va_list ap;
   va_start(ap, format);
   vprintf(format, ap);
   va_end(ap);
}

static const char* toDec(int8_t value_)
{
   static char text[4];

   value_ = value_ * 99 / 127;
   snprintf(text, sizeof(text), "%u.%u", value_ / 10, value_ % 10);
   return text;
}

void Juno106::SysEx::print(const char* name_)
{
   printf("\n");

   pretf("PATCH", " %s ", name_);

   pretf("LFO");
   pretf(" rate ", "%s", toDec(lfo_rate));
   pretf(" dly ", "%s\n", toDec(lfo_delay));

   pretf("DCO",     " %s", range == RANGE_16 ? "16'" : range == RANGE_8 ? "8' " : "4' ");
   pretf(" lfo ",   "%s", toDec(dco_lfo));
   pretf(" pwm ",   "%s %s %s", pwm ? "ON " : "OFF", toDec(dco_pwm), dco_pwm_man ? "MAN" : "LFO");
   pretf(" ramp ",  "%s", saw ? "ON " : "OFF");
   pretf(" sub ",   "%s", toDec(sub_osc_level));
   pretf(" noise ", "%s\n", toDec(noise_level));

   pretf("HPF", " %s ", hpf == HPF_0 ? "0" : hpf == HPF_1 ? "1" : hpf == HPF_2 ? "2" : "3");

   pretf("VCF");
   pretf(" frq ", "%s", toDec(vcf_freq));
   pretf(" res ", "%s", toDec(vcf_res));
   pretf(" env ", "%c%s", vcf_neg ? '-' : '+', toDec(vcf_env));
   pretf(" lfo ", "%s", toDec(vcf_lfo));
   pretf(" kbd ", "%s\n", toDec(vcf_kbd));

   pretf("VCA", " %s %s ", vca_gate ? "GATE" : "ENV", toDec(vca_level - 0x40));

   pretf("ENV");
   pretf(" A ", "%s", toDec(env_attack));
   pretf(" D ", "%s", toDec(env_decay));
   pretf(" S ", "%s", toDec(env_sustain));
   pretf(" R ", "%s  ", toDec(env_release));

   pretf("CHORUS", " %s\n", chorus_off ? "OFF" : chorus_1 ? " I " : "II ");

   printf("\n");
}
