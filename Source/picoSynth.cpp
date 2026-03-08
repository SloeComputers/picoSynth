//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Synth for Raspberry Pi Pico

#include <cstdio>
#include <unistd.h>

#include "Hardware/picoX7/Config.h"
#include "Hardware/FilePortal.h"

#include "SIG/SIG.h"

#include "SynthRack.h"

// -----------------------------------------------------------------------------

static const bool     MIDI_DEBUG = false;  //!< Enable MIDI debug on console
static const bool     PROFILE    = false;  //!< Enable profiling on LCD
static const unsigned TICK_RATE  = 800;    //!< 800 Hz

static SynthRack rack{};
static Synth*    synth{};

// -----------------------------------------------------------------------------

static hw::Profiler<PROFILE> profiler_core0{};
static hw::Profiler<PROFILE> profiler_core1{};
static hw::PhysMidi          phys_midi{};
static hw::Led7Seg           led_7seg;
static hw::Lcd               lcd{};            //!< 16x2 LCD
static hw::Led               led{};
static hw::Buttons           buttons{/* irq */ false};

extern "C" void IRQ_IO_BANK0() { buttons.irq(); }


// --- USB MIDI and FILE -------------------------------------------------------

static hw::FilePortal file_portal{"picoSynth",
                                  "https://github.com/SloeComputers/picoSynth"};

static hw::UsbFileMidi usb{0xD157, "picoSynth", file_portal};

extern "C" void IRQ_USBCTRL() { usb.irq(); }


// --- DAC ---------------------------------------------------------------------

static const unsigned SAMPLES_PER_TICK = SIG::SAMPLE_RATE / TICK_RATE;  //!< DAC buffer size (16 bit samples)

static hw::Audio<SAMPLES_PER_TICK> audio{SIG::SAMPLE_RATE};

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
   synth = rack.get();

   usb.attachInstrument(1, *synth);
   phys_midi.attachInstrument(1, *synth);

   // XXX the AKAI MPK mini MIDI controller sends
   //     program changes on MIDI channel 2 #!@*4%
   usb.attachInstrument(2, *synth);
   phys_midi.attachInstrument(2, *synth);
}

int main()
{
   // Clear screen and cursor to home
   printf("\033[2J\033[H");

   printf("\n");
   puts(file_portal.addREADME("picoSynth"));
   printf("\n");

   lcd.print("   pico-Synth   ");

   usleep(1000000);

   usb.setDebug(MIDI_DEBUG);
   phys_midi.setDebug(MIDI_DEBUG);

   rack.select(0);

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

      unsigned number{};
      if (synth->getNumber(number))
      {
         led_7seg.printDec(number, number >= 100 ? 0 : 3);
      }

      bool     down{};
      unsigned index{};
      if (buttons.popEvent(index, down) && down)
      {
         rack.next();

         initSynth();
      }

      usleep(100000);
   }

   return 0;
}
