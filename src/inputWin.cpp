#include "inputWin.h"
#include <conio.h> // for _getch
#include <stdio.h> // for getchar
#include <Windows.h> // for sleep
#include <iostream>

using std::cout;
using std::endl;

using namespace InputNS;

InputWin::InputWin()
{
  currentMode = INPUT_DEFAULT;

  // make input stucture

  int inputSize = 4;
  in = new INPUT[inputSize];

  initInKeyboard.type = INPUT_KEYBOARD;
  initInKeyboard.ki.wVk = 0;
  initInKeyboard.ki.wScan = 0;
  initInKeyboard.ki.dwFlags = 0;
  initInKeyboard.ki.time = 0;
  initInKeyboard.ki.dwExtraInfo = 0;

  for (int i = 0; i < inputSize; i++)
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

// TODO delete me
//int InputWin::charToCode(char c)
//{
//  return 0;
//}

int InputWin::getModifiers()
  // return state of modifiers
  // bits: LSHIFT, RSHIFT, LCONTROL, RCONTROL, LMENU, RMENU
{
  int modifiers = 0;

  // if most significant bit is set, key is down
  SHORT keyinfo = GetAsyncKeyState(VK_LCONTROL);
  cout << "keyinfo: " << keyinfo << endl;
  if (((keyinfo >> 8) & 128) == 128) modifiers |= MODIFIER_LCONTROL;

  keyinfo = GetAsyncKeyState(VK_RCONTROL);
  if (((keyinfo >> 8) & 128) == 128) modifiers |= MODIFIER_RCONTROL;

  return modifiers;
}

int InputWin::sendKeytoOS(char key)
  // this deals with upper and lower case chars, i.e. by converting to virtual keys
  // and sending shift if necessary
{
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

  if (shiftState == 0) {
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
  return 0;
}

int InputWin::sendHardcoretoOS(Hardcore key)
  // this just sends the virtual key, nothing fancy, will not auto-keyup
{
  return 0;
}

