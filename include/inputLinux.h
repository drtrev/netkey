#ifndef _MSC_VER
#ifndef INPUTLINUX_H
#define INPUTLINUX_H

#include "input.h"
#include <termios.h>

class InputLinux : public Input {
  private:
    struct termios oldt, newt;

  public:
    void changeMode(InputNS::InputMode dir);
    int kbhit();
    int getChin();

    // Do everything for you:
    // sets mode to interactive, waits for keypress,
    // then resets mode to default
    int waitKey();
    int charToCode(char);
    int getModifiers();

    int sendKeytoOS(char key);
    int sendKeyCodetoOS(int);
};

#endif
#endif
