#if defined _MSC_VER || defined MSYS
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
  // note that ctrl or shift changes character input, i.e. control characters
  // Ctrl+A = 1, Ctrl+B = 2 etc
  // and VkKeyScan(key) can successfully work this out at the other end
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

  keyinfo = GetAsyncKeyState(VK_LSHIFT);
  if (((keyinfo >> 8) & 128) == 128) modifiers |= MODIFIER_LSHIFT;

  keyinfo = GetAsyncKeyState(VK_RSHIFT);
  if (((keyinfo >> 8) & 128) == 128) modifiers |= MODIFIER_RSHIFT;

  keyinfo = GetAsyncKeyState(VK_LMENU);
  if (((keyinfo >> 8) & 128) == 128) modifiers |= MODIFIER_LMENU;

  keyinfo = GetAsyncKeyState(VK_RMENU);
  if (((keyinfo >> 8) & 128) == 128) modifiers |= MODIFIER_RMENU;

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
  if (mods & MODIFIER_RCONTROL) {
    // put in input structure
    in[inlen] = initInKeyboard;
    in[inlen].ki.wVk = VK_RCONTROL;
    if (!down) in[inlen].ki.dwFlags = KEYEVENTF_KEYUP;
    cout << "Sending RCONTROL ";
    if (down) cout << "down" << endl;
    else cout << "up" << endl;
    inlen++;
  }
  if (mods & MODIFIER_LSHIFT) {
    // put in input structure
    in[inlen] = initInKeyboard;
    in[inlen].ki.wVk = VK_LSHIFT;
    if (!down) in[inlen].ki.dwFlags = KEYEVENTF_KEYUP;
    cout << "Sending LSHIFT ";
    if (down) cout << "down" << endl;
    else cout << "up" << endl;
    inlen++;
  }
  if (mods & MODIFIER_RSHIFT) {
    // put in input structure
    in[inlen] = initInKeyboard;
    in[inlen].ki.wVk = VK_RSHIFT;
    if (!down) in[inlen].ki.dwFlags = KEYEVENTF_KEYUP;
    cout << "Sending RSHIFT ";
    if (down) cout << "down" << endl;
    else cout << "up" << endl;
    inlen++;
  }
  if (mods & MODIFIER_LMENU) {
    // put in input structure
    in[inlen] = initInKeyboard;
    in[inlen].ki.wVk = VK_LMENU;
    if (!down) in[inlen].ki.dwFlags = KEYEVENTF_KEYUP;
    cout << "Sending LMENU ";
    if (down) cout << "down" << endl;
    else cout << "up" << endl;
    inlen++;
  }
  if (mods & MODIFIER_RMENU) {
    // put in input structure
    in[inlen] = initInKeyboard;
    in[inlen].ki.wVk = VK_RMENU;
    if (!down) in[inlen].ki.dwFlags = KEYEVENTF_KEYUP;
    cout << "Sending RMENU ";
    if (down) cout << "down" << endl;
    else cout << "up" << endl;
    inlen++;
  }

  if (inlen > MAX_INPUT) { cerr << "inlen too big" << endl; }else
  SendInput(inlen, in, sizeof(INPUT));
  return 0;
}

int InputWin::sendKeytoOS(char key, bool performCtrlShift)
  // this deals with upper and lower case chars, and control i.e. by converting to virtual keys
  // and sending shift if necessary (control+key sends control character, which is correctly identified by VkKeyScan)
  // This won't get ctrl and shift if input is _getch - it returns the control code
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
  //WORD vk = vkscan;
  cout << "vk after convert: " << vk << endl;

  int inlen = 0;

  if (shiftState == 0 || !performCtrlShift) {
    // put in input structure
    in[0] = initInKeyboard;
    in[0].ki.wVk = vk; // key down

    in[1] = initInKeyboard;
    in[1].ki.wVk = vk; // key up
    in[1].ki.dwFlags = KEYEVENTF_KEYUP;

    inlen = 2;
  }else{
    int curin = 0;

    // put in input structure
    if (shiftState & 1) {
      cout << "Sending shift" << endl;
      in[curin] = initInKeyboard;
      in[curin].ki.wVk = VK_SHIFT; // shift down
      curin++;
    }
    if (shiftState & 2) {
      cout << "Sending control" << endl;
      in[curin] = initInKeyboard;
      in[curin].ki.wVk = VK_CONTROL; // ctrl down
      curin++;
    }

    in[curin] = initInKeyboard;
    in[curin].ki.wVk = vk;       // key down

    curin++;

    in[curin] = initInKeyboard;
    in[curin].ki.wVk = vk;       // key up
    in[curin].ki.dwFlags = KEYEVENTF_KEYUP;

    curin++;

    if (shiftState & 1) {
      cout << "Shift up" << endl;
      in[curin] = initInKeyboard;
      in[curin].ki.wVk = VK_SHIFT; // shift up
      in[curin].ki.dwFlags = KEYEVENTF_KEYUP;
      curin++;
    }

    if (shiftState & 2) {
      cout << "Control up" << endl;
      in[curin] = initInKeyboard;
      in[curin].ki.wVk = VK_CONTROL; // shift up
      in[curin].ki.dwFlags = KEYEVENTF_KEYUP;
      curin++;
    }

    inlen = curin;
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

#endif
