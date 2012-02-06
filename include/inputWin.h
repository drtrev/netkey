#ifndef INPUTWIN_H
#define INPUTWIN_H

#include "input.h"
#include "Windows.h"
#include "Winuser.h"

class InputWin : public Input {
  private:
    INPUT myinput;

  public:
    InputWin();
    ~InputWin();

    void changeMode(InputNS::InputMode mode);
    int kbhit();
    int getChin();

    // Do everything for you:
    // sets mode to interactive, waits for keypress,
    // then resets mode to default
    int waitKey();

    int sendKeytoOS(char key);
};

#endif
