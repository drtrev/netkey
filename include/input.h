#ifndef INPUT_H
#define INPUT_H

namespace InputNS { 
  enum InputMode { INPUT_DEFAULT, INPUT_INTERACTIVE };
  enum Modifiers { MODIFIER_LSHIFT = (1 << 6), MODIFIER_RSHIFT = (1 << 5), MODIFIER_LCONTROL = (1 << 4), MODIFIER_RCONTROL = (1 << 3), MODIFIER_LMENU = (1 << 2), MODIFIER_RMENU = 1 };
  enum Hardcore { H_1 };
}

class Input {
  private:

  public:
    virtual ~Input();

    // ------------------------------------------------
    // --- Basic character input from terminal ---
    // gets a character without waiting for enter
    //
    // Should first call changemode(InputNS::INPUT_INTERACTIVE)
    // then selectStdin check's if we've got a keypress (if STDIN_FILENO is readable)
    // so while(!selectStdin()) { do something else; usleep(100000); } mychar = getchin();
    // When finished, call changemode(InputNS::INPUT_DEFAULT)

    virtual void changeMode(InputNS::InputMode dir) = 0;
    virtual int selectStdin() = 0; // like kbhit, check's if anythings on the input stream
    virtual int getChin() = 0; // get character input (this is a function because it's slightly different on linux and windows)

    // TODO getChin blocks so this is pointless:
    // Do everything for you:
    // sets mode to interactive, waits for keypress,
    // then resets mode to default
    //virtual int waitChin() = 0;

    // ------------------------------------------------
    // --- hardcore input ---
    // get exact key pressed, even if it's CTRL or ALT
    virtual int getHardcoreKey(InputNS::Hardcore &key) = 0;

    // ------------------------------------------------
    // --- somewhere in the middle
    // convert from char to hardcore
    // get current modifiers
    //virtual int charToCode(char) = 0;
    //virtual int hardcoreToChar(InputNS::Hardcore key) = 0;
    virtual int getModifiers() = 0;

    // ------------------------------------------------
    // --- Sending to OS from character
    // this sorts out characters, including pressing shift if necessary, and keyup afterwards
    virtual int sendKeytoOS(char key) = 0;

    // --- Senidng to OS from hardcore
    // this transmits an OS specific code (e.g. virtual key on windows)
    // which is read from charToCode and getModifiers. In future could have a direct waitKeyCode() or something.
    virtual int sendHardcoretoOS(InputNS::Hardcore key) = 0;
};

#endif

