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
    InputNS::InputMode currentMode;
    int MAX_INPUT;

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

    //int charToCode(char);
    char getModifiers();

    int sendModifierstoOS(char mods, bool down);
    int sendKeytoOS(char key, bool);
    int sendHardcoretoOS(InputNS::Hardcore key);
};

#endif
