//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "STB/MIDIInstrument.h"

#include "MidiController.h"
#include "MidiMap.h"
#include "Control.h"
#include "NoteButton.h"

//! Synth external interface
class Synth
   : public MIDI::Instrument
   , public MidiMap::ControlMap
{
public:
   Synth(unsigned num_voices_)
      : MIDI::Instrument(num_voices_)
   {
   }

   void init()
   {
      setNumber(0);

      char title[17];
      MidiMap::centerText(title, 16, name);

      setText(0, title);
      setText(1, "");

      synthInit();

      dumpControls();

      for(unsigned i = 0; i < num_voices; ++i)
      {
         voiceOff(i, 0);
      }

      voiceProgram(0, 0);
   }

   //! Get stereo samples from synth
   virtual void getSamples(uint32_t* buffer, unsigned n) = 0;

   //! Get display text for the given line if it has been updated
   const char* getText(unsigned line_)
   {
      if (not text_update[line_])
         return nullptr;

      text_update[line_] = false;

      return &text[line_][0];
   }

   //! Get display number if it has been updated
   bool getNumber(unsigned& number_)
   {
      if (not number_update)
         return false;

      number_       = number;
      number_update = false;
      return true;
   }

protected:
   virtual void synthInit()
   {
   }

   virtual void synthProgram(uint8_t num_)
   {
      synthEdit();
   }

   virtual void synthEdit()
   {
   }

   virtual void synthControl(uint8_t midi_control_, uint8_t value_)
   {
      for(unsigned i = 0; i < num_control; ++i)
      {
         const Control& ctrl = control[i];
         char           text[17];

         if (ctrl.edit(midi_control_, value_, text, sizeof(text)))
         {
            setText(1, text);
            synthEdit();
            break;
         }
      }
   }

   virtual bool synthFilterNote(uint8_t midi_note_)
   {
      for(unsigned i = 0; i < num_button; ++i)
      {
         const NoteButton& btn = button[i];
         const char*       text{};

         if (btn.edit(midi_note_, text))
         {
            setText(1, text);
            synthEdit();
            return true;
         }
      }

      return false;
   }

   void voiceProgram(unsigned index_, uint8_t num_) override
   {
      if (index_ != 0)
         return;

      synthProgram(num_);
   }

   void voiceControl(unsigned index_, uint8_t control_, uint8_t value_) override
   {
      if (index_ != 0)
         return;

      synthControl(control_, value_);
   }

   void configure(const char* name_)
   {
      name = name_;
   }

   //! Add a MIDI variable control
   template <typename TYPE>
   void addCtrl(uint8_t     midi1_,
                uint8_t     midi2_,
                TYPE        min_,
                TYPE        max_,
                const char* name_,
                const char* unit_,
                TYPE&       patch_,
                unsigned    width_ = 0,
                unsigned    lsb_   = 0)
   {
      Control& ctrl = control[num_control++];

      ctrl.init<TYPE>(midi1_, midi2_, min_, max_, name_, unit_, nullptr, patch_, width_, lsb_);
   }

   //! Add a MIDI variable control
   template <typename TYPE>
   void addCtrl(uint8_t              midi1_,
                uint8_t              midi2_,
                unsigned             n_,
                const Control::Enum* enum_,
                const char*          name_,
                TYPE&                patch_,
                unsigned             width_ = 0,
                unsigned             lsb_   = 0)
   {
      Control& ctrl = control[num_control++];

      ctrl.init<TYPE>(midi1_, midi2_, TYPE(0), TYPE(n_ - 1), name_, "", enum_, patch_, width_, lsb_);
   }

   //! Add a MIDI note button control
   void addBtn(uint8_t     note_up_,
               uint8_t     note_dn_,
               uint8_t     max_,
               const char* enum_table_[],
               uint8_t&    patch_,
               unsigned    width_ = 8,
               unsigned    lsb_   = 0)
   {
      NoteButton& btn = button[num_button++];

      btn.init(note_up_, note_dn_, max_, enum_table_, patch_, width_, lsb_);
   }

   //! Add a MIDI note toggle button control
   void addTgl(uint8_t     note_,
               const char* enum_table_[],
               uint8_t&    patch_,
               unsigned    bit_pos_ = 0)
   {
      addBtn(note_, note_, /* max */ 1, enum_table_, patch_, /* width */ 1, bit_pos_);
   }

   //! Update text for the given line
   void setText(unsigned line_, const char* text_)
   {
      text_update[line_] = false;

      unsigned i = 0;

      for(; (i < MAX_TEXT_LEN) && (text_[i] != '\0'); ++i)
         text[line_][i] = text_[i];

      for(; i < MAX_TEXT_LEN; ++i)
         text[line_][i] = ' ';

      text[line_][i]     = '\0';
      text_update[line_] = true;
   }

   void setNumber(unsigned number_)
   {
      number = number_;
      number_update = true;
   }

private:
   const char* findControl(uint8_t midi, unsigned n) const override
   {
      for(unsigned i = 0; i < num_control; ++i)
      {
         const char* label = control[i].checkLabel(midi, n);
         if (label != nullptr)
            return label;
      }

      return nullptr;
   }

   void dumpControls()
   {
      printf("\n--------------------------------------------------------------------------------\n");
      printf("%s\n\n", name);

      MidiMap::print("Akai MIDImix", Akai::MIDImix::map, this, 1);
      MidiMap::print("Akai MPKmini", Akai::MPKmini::map, this, 2);
   }

   static const unsigned MAX_TEXT_LEN   = 16;
   static const unsigned MAX_TEXT_LINES = 2;
   static const unsigned MAX_CONTROL    = 33;
   static const unsigned MAX_BUTTON     = 12;

   const char* name{""};

   char text[MAX_TEXT_LINES][MAX_TEXT_LEN + 1];
   bool text_update[MAX_TEXT_LINES] = {};

   unsigned number{0};
   unsigned number_update{false};

   unsigned num_control{0};
   Control  control[MAX_CONTROL];

   unsigned   num_button{0};
   NoteButton button[MAX_BUTTON];
};
