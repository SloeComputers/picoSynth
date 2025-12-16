//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Synth for Raspberry Pi Pico

#include <cstdio>
#include <unistd.h>

#include "Hardware/picoX7/Config.h"
#include "Hardware/FilePortal.h"

#include "SIG/Sample.h"

#include "Juno106/Synth.h"
#include "VL1/Synth.h"
#include "Simple/Synth.h"


static const unsigned DAC_FREQ         = SAMPLE_RATE;
static const unsigned TICK_RATE        = 400;                   //!< 400 Hz
static const unsigned SAMPLES_PER_TICK = DAC_FREQ / TICK_RATE;  //!< DAC buffer size (16 bit samples)
static const bool     MIDI_DEBUG       = false;
static const bool     PROFILE          = false;
static const unsigned NUM_SYNTHS       = 3;


static Juno106::Synth juno106{};
static VL1::Synth     vl1{};
static Simple::Synth  simple{};
static Synth*         synth{};
static unsigned       synth_index{0};


// -----------------------------------------------------------------------------

static hw::Profiler<PROFILE> profiler_core0{};
static hw::Profiler<PROFILE> profiler_core1{};
static hw::PhysMidi          phys_midi{};
static hw::Lcd               lcd{};            //!< 16x2 LCD
static hw::Led               led{};
static hw::Buttons           buttons{/* irq */ false};

extern "C" void IRQ_IO_BANK0() { buttons.irq(); }


// --- USB MIDI and FILE -------------------------------------------------------

static hw::FilePortal file_portal{"dinkySynth",
                                  "https://github.com/AnotherJohnH/dinkySynth"};

static hw::UsbFileMidi usb{0xD157, "dinkySynth", file_portal};

extern "C" void IRQ_USBCTRL() { usb.irq(); }


// --- DAC ---------------------------------------------------------------------

static hw::Audio<SAMPLES_PER_TICK> audio{DAC_FREQ};

static void hwTick();

#if defined(HW_DAC_I2S) || defined(HW_DAC_PWM)

MTL_AUDIO_ATTACH_IRQ_0(audio);

void MTL::Audio::getSamples(uint32_t* buffer, unsigned n)
{
   profiler_core0.start();

   synth->getSamples(buffer, n);

   hwTick();

   profiler_core0.stop();
}

#else

template<>
void hw::Audio<SAMPLES_PER_TICK>::getSamples32(uint32_t* buffer, unsigned n)
{
   profiler_core0.start();

   synth->getSamples(buffer, n);

   hwTick();

   profiler_core0.stop();
}

#endif

// -----------------------------------------------------------------------------

static void hwTick()
{
   phys_midi.tick();
   usb.tick();
}

void profileReport()
{
   char text[32];

   lcd.move(0, 0);
   lcd.print(profiler_core0.format(text));

   lcd.move(0, 1);
   lcd.print(profiler_core1.format(text));
}

void initSynth()
{
   switch(synth_index)
   {
   case 0: synth = &juno106; break;
   case 1: synth = &vl1;     break;
   case 2: synth = &simple;  break;
   }

   usb.attachInstrument(1, *synth);
   phys_midi.attachInstrument(1, *synth);

   // XXX the AKAI MPK mini MIDI controller sends
   //     program changes on MIDI channel 2 #!@*4%
   usb.attachInstrument(2, *synth);
   phys_midi.attachInstrument(2, *synth);

   synth->init();
}

int main()
{
   // Clear screen and cursor to home
   printf("\033[2J\033[H");

   printf("\n");
   puts(file_portal.addREADME("dinkySynth"));
   printf("\n");

   lcd.print(">> dinkySynth <<");

   usleep(1000000);

   usb.setDebug(MIDI_DEBUG);
   phys_midi.setDebug(MIDI_DEBUG);

   initSynth();

   audio.start();

   while(true)
   {
      led = synth->isAnyVoiceOn();

      if (PROFILE)
         profileReport();
      else
      {
         for(unsigned line = 0; line < 2; ++line)
         {
            const char* text = synth->getText(line);
            if (text != nullptr)
            {
               lcd.move(0, line);
               lcd.print(text);
            }
         }
      }

      bool     down{};
      unsigned index{};

      if (buttons.popEvent(index, down))
      {
         if (down)
         {
            synth_index = (synth_index + 1) % NUM_SYNTHS;

            initSynth();
         }
      }

      usleep(100000);
   }

   return 0;
}
