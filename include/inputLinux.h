#ifndef _MSC_VER
#ifndef INPUTLINUX_H
#define INPUTLINUX_H

#include "input.h"
#include <termios.h>

class InputLinux : public Input {
  private:
    struct termios oldt, newt;

  public:
    void changeMode(InputNS::InputMode mode);
    int selectStdin();
    int getChin();

    // Do everything for you:
    // sets mode to interactive, waits for keypress,
    // then resets mode to default
    //int waitChin();
    //
    //
    int getHardcoreKey(InputNS::Hardcore &key);

    //int charToCode(char);
    char getModifiers();

    int sendKeytoOS(char key);
    int sendHardcoretoOS(InputNS::Hardcore key);
};

#endif
#endif
