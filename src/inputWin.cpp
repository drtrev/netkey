#include "inputWin.h"
#include <conio.h>
#include <Windows.h> // for sleep
#include <iostream>

using std::cout;
using std::endl;

InputWin::InputWin()
{
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

void InputWin::changeMode(InputNS::InputMode mode)
{
}

int InputWin::kbhit()
{
  if (_kbhit()) return 1;
  else return 0;
}

int InputWin::getChin()
{
  int c = _getch();
  if (c == 13) c = 10; // convert carriage return to newline
  return c;
}

int InputWin::waitKey()
{
  while (!_kbhit()) Sleep(100);

  return getChin();
}

int InputWin::sendKeytoOS(char key)
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
    in[0].ki.wVk = vk;
    in[0].ki.dwFlags = 0;
    in[1].ki.wVk = vk;
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

