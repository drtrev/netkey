#ifndef INPUTWIN_H
#define INPUTWIN_H

#include "input.h"

class InputWin : public Input {
  private:

  public:

    void changeMode(InputNS::InputMode mode);
    int kbhit();
    int getChin();

    // Do everything for you:
    // sets mode to interactive, waits for keypress,
    // then resets mode to default
    int waitKey();
};

#endif
