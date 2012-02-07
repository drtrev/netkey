#include "inputWin.h"
#include <conio.h> // for _getch
#include <stdio.h> // for getchar
#include <Windows.h> // for sleep
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

using namespace InputNS;

InputWin::InputWin()
{
  currentMode = INPUT_DEFAULT;

  // make input stucture

  MAX_INPUT = 6;
  in = new INPUT[MAX_INPUT];

  initInKeyboard.type = INPUT_KEYBOARD;
  initInKeyboard.ki.wVk = 0;
  initInKeyboard.ki.wScan = 0;
  initInKeyboard.ki.dwFlags = 0;
  initInKeyboard.ki.time = 0;
  initInKeyboard.ki.dwExtraInfo = 0;

  for (int i = 0; i < MAX_INPUT; i++)
    in[i] = initInKeyboard;
}

InputWin::~InputWin()
{
  delete [] in;
}

void InputWin::changeMode(InputMode mode)
{
  currentMode = mode;
}

int InputWin::selectStdin()
{
  if (_kbhit()) return 1;
  else return 0;
}

int InputWin::getChin()
{
  int c;
  if (currentMode == INPUT_INTERACTIVE) {
    c = _getch();
  }else{
    c = getchar(); // behave like linux in canonical mode - wait for newline
  }
  if (c == 13) c = 10; // convert carriage return to newline
  return c;
}

// eh? getChin blocks
//int InputWin::waitKey()
//{
 // while (!_kbhit()) Sleep(100);

  //return getChin();
//}

//int InputWin::charToHardcore(char c)
//{
//  return 0;
//}

char InputWin::getModifiers()
  // return state of modifiers
  // bits: LSHIFT, RSHIFT, LCONTROL, RCONTROL, LMENU, RMENU
{
  char modifiers = 0;

  // if most significant bit is set, key is down
  SHORT keyinfo = GetAsyncKeyState(VK_LCONTROL);
  cout << "keyinfo: " << keyinfo << endl;
  if (((keyinfo >> 8) & 128) == 128) modifiers |= MODIFIER_LCONTROL;

  keyinfo = GetAsyncKeyState(VK_RCONTROL);
  if (((keyinfo >> 8) & 128) == 128) modifiers |= MODIFIER_RCONTROL;

  return modifiers;
}

int InputWin::sendModifierstoOS(char mods, bool down)
{
  int inlen = 0;
  if (mods & MODIFIER_LCONTROL) {
    // put in input structure
    in[inlen] = initInKeyboard;
    in[inlen].ki.wVk = VK_LCONTROL;
    if (!down) in[inlen].ki.dwFlags = KEYEVENTF_KEYUP;
    cout << "Sending LCONTROL ";
    if (down) cout << "down" << endl;
    else cout << "up" << endl;
    inlen++;
  }

  if (inlen > MAX_INPUT) { cerr << "inlen too big" << endl; }else
  SendInput(inlen, in, sizeof(INPUT));
  return 0;
}

int InputWin::sendKeytoOS(char key, bool performShift)
  // this deals with upper and lower case chars, i.e. by converting to virtual keys
  // and sending shift if necessary
{
  int temp = (int) key;
  cout << "sendKeytoOS: key: " << temp << endl;
  // convert to virtual key
  // (use VkKeyScanEx to specify keyboard layout)
  SHORT vkscan = VkKeyScan(key);

  // lower-order byte contains virtual key code
  // higher-order byte contains shift state
  int shiftState = vkscan >> 8;
  cout << "shiftState: " << shiftState << endl;
  cout << "vk before convert: " << vkscan << endl;
  WORD vk = vkscan & 255;
  cout << "vk after convert: " << vk << endl;

  int inlen = 0;

  if (shiftState == 0 || !performShift) {
    // put in input structure
    in[0] = initInKeyboard;
    in[0].ki.wVk = vk; // key down

    in[1] = initInKeyboard;
    in[1].ki.wVk = vk; // key up
    in[1].ki.dwFlags = KEYEVENTF_KEYUP;

    inlen = 2;
  }else{
    cout << "Sending shift" << endl;

    // put in input structure
    in[0] = initInKeyboard;
    in[0].ki.wVk = VK_SHIFT; // shift down

    in[1] = initInKeyboard;
    in[1].ki.wVk = vk;       // key down

    in[2] = initInKeyboard;
    in[2].ki.wVk = vk;       // key up
    in[2].ki.dwFlags = KEYEVENTF_KEYUP;

    in[3] = initInKeyboard;
    in[3].ki.wVk = VK_SHIFT; // shift up
    in[3].ki.dwFlags = KEYEVENTF_KEYUP;

    inlen = 4;
  }

  // send
  SendInput(inlen, in, sizeof(INPUT));

  return 0;
}

int InputWin::getHardcoreKey(Hardcore &key)
{
  // this needs a window, according to:
  // http://msdn.microsoft.com/en-us/library/windows/desktop/ms644928%28v=vs.85%29.aspx#creating_loop
  /*MSG msg;

  // change the filter min/max, i.e. KEYFIRST KEYLAST if want other messages
  // e.g. change them both to WM_INPUT, see msdn docs
  // Returns 0 on WM_QUIT, this is never filtered
  BOOL ret; // this can be -1 0 or 1, what the hell kind of bool is that??
  
  cout << "Starting getmessage" << endl;
  ret = GetMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST);
  cout << "getmessage returned: " << ret << endl;
  if (ret > 0) {
    switch(msg.message) {
      case WM_KEYDOWN:
        cout << "keydown: " << (int) msg.wParam << endl;
        break;
      case WM_KEYUP:
        cout << "keyup: " << (int) msg.wParam << endl;
        break;
    }
  }*/

  // This also needs a window, because it relies on changes to the message queue
  /*BYTE keystate[256]; // TODO put as member var on heap
  if (GetKeyboardState(keystate) == 0) {
    cerr << "Error getting keyboard state. Use GetLastError" << endl;
  }else{
    // do something
    cout << "Got keystate" << endl;
    cout << "41: " << ((int) keystate[41]) << endl;
  }*/


  return 0;
}

int InputWin::sendHardcoretoOS(Hardcore key)
  // this just sends the virtual key, nothing fancy, will not auto-keyup
{
  return 0;
}

