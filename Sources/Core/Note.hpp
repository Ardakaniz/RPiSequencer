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
      Note(unsigned int note, float duration);
			Note(const Note& note, int offset);

			int Offset(const Note& other) const;
			bool IsBlankNote() const;
			float GetDuration() const;

  private:
      unsigned int _note;
			float _duration;
  };
}

#endif // CORE_NOTE_HPP
