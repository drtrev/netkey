#ifndef _MSC_VER
#ifndef INPUTLINUX_H
#define INPUTLINUX_H

#include "input.h"
#include <termios.h>

class InputLinux : public Input {
  private:
    struct termios oldt, newt;

  public:
    void changemode(InputNS::InputMode dir);
    int kbhit();
    int getchin();

    // Do everything for you:
    // sets mode to interactive, waits for keypress,
    // then resets mode to default
    int getkeypress();
};

#endif
#endif
