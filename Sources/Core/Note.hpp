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
      Note(unsigned int note);

  private:
      bool _is_blank_note;
      unsigned int _note;
  };
}

#endif // CORE_NOTE_HPP
