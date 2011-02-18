#ifndef INPUT_H
#define INPUT_H

namespace InputNS { 
  enum InputMode { INPUT_DEFAULT, INPUT_INTERACTIVE };
}

class Input {
  private:

  public:
    virtual ~Input();

    virtual void changemode(InputNS::InputMode dir) = 0;
    virtual int kbhit() = 0;
    virtual int getchin() = 0; // get character input

    // Do everything for you:
    // sets mode to interactive, waits for keypress,
    // then resets mode to default
    virtual int getkeypress() = 0;
};

#endif

