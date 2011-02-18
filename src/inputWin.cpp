#include "inputWin.h"
#include <conio.h>
#include <Windows.h> // for sleep
#include <iostream>

using std::cout;
using std::endl;

void InputWin::changemode(InputNS::InputMode mode)
{
}

int InputWin::kbhit()
{
  if (_kbhit()) return 1;
  else return 0;
}

int InputWin::getchin()
{
  int c = _getch();
  if (c == 13) c = 10; // convert carriage return to newline
  return c;
}

int InputWin::getkeypress()
{
  while (!_kbhit()) Sleep(100);

  return getchin();
}
