#pragma once

#ifndef CORE_NOTE_HPP
#define CORE_NOTE_HPP

#include <string>

namespace Core
{
  class Note
  {
  public:
      Note();
      Note(char note, unsigned int octave);
      Note(const std::string& note);

  private:
      [[noreturn]] void ThrowInvalidNoteString();

      bool _is_blank_note;
      char _note;
      unsigned int _octave;
  };
}

#endif // CORE_NOTE_HPP