#ifndef INPUT_H
#define INPUT_H

namespace InputNS { 
  enum InputMode { INPUT_DEFAULT, INPUT_INTERACTIVE };
  enum Modifiers { MODIFIER_LSHIFT = (1 << 6), MODIFIER_RSHIFT = (1 << 5), MODIFIER_LCONTROL = (1 << 4), MODIFIER_RCONTROL = (1 << 3), MODIFIER_LMENU = (1 << 2), MODIFIER_RMENU = 1 };
}

class Input {
  private:

  public:
    virtual ~Input();

    // Should first call changemode(InputNS::INPUT_INTERACTIVE)
    // then kbhit check's if we've got a keypress (if STDIN_FILENO is readable)
    // so while(!kbhit()) { usleep(100000); } mychar = getchin();
    // When finished, call changemode(InputNS::INPUT_DEFAULT)

    virtual void changeMode(InputNS::InputMode dir) = 0;
    virtual int kbhit() = 0;
    virtual int getChin() = 0; // get character input (this is a function because it's slightly different on linux and windows)

    // Do everything for you:
    // sets mode to interactive, waits for keypress,
    // then resets mode to default
    virtual int waitKey() = 0;

    virtual int charToCode(char) = 0;
    virtual int getModifiers() = 0;

    // this sorts out characters, including pressing shift if necessary, and keyup afterwards
    virtual int sendKeytoOS(char key) = 0;
    // this transmits an OS specific code (e.g. virtual key on windows)
    // which is read from charToCode and getModifiers. In future could have a direct waitKeyCode() or something.
    virtual int sendKeyCodetoOS(int vk) = 0;
};

#endif

