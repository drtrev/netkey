#ifndef INPUT_H
#define INPUT_H

namespace InputNS { 
  enum InputMode { INPUT_DEFAULT, INPUT_INTERACTIVE };
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
};

#endif

