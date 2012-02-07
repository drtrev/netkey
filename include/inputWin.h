#ifndef INPUTWIN_H
#define INPUTWIN_H

#include "input.h"
#include "Windows.h"
// The documentation says we need winuser, but we need winable
//#include "Winuser.h"
#include "WinAble.h"

class InputWin : public Input {
  private:
    INPUT *in;
    INPUT initInKeyboard;

  public:
    InputWin();
    ~InputWin();

    void changeMode(InputNS::InputMode mode);
    int selectStdin();
    int getChin();

    // Do everything for you:
    // sets mode to interactive, waits for keypress,
    // then resets mode to default
    //int waitKey();

    int getHardcoreKey(InputNS::Hardcore &key);

    int charToCode(char);
    int getModifiers();

    int sendKeytoOS(char key);
    int sendHardcoretoOS(InputNS::Hardcore key);
};

#endif
