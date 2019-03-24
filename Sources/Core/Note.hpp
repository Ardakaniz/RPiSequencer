#pragma once

#ifndef CORE_NOTE_HPP
#define CORE_NOTE_HPP

#include "Platform/MidiNote.hpp"

namespace Core
{
  class Note
  {
    friend Platform::MidiNote;
    
  public:
      Note();
      Note(unsigned int note);
			Note(const Note& note, int offset);

			int Offset(const Note& other) const;

  private:
      bool _is_blank_note;
      unsigned int _note;
  };
}

#endif // CORE_NOTE_HPP
