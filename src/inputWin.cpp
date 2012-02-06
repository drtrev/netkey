#include "inputWin.h"
#include <conio.h>
#include <Windows.h> // for sleep
#include <iostream>

using std::cout;
using std::endl;

InputWin::InputWin()
{
  
}

InputWin::~InputWin()
{
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

  return 0;
}

