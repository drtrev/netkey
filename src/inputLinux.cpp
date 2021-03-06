// From http://cboard.cprogramming.com/linux-programming/51531-faq-cached-input-mygetch.html

#if !defined _MSC_VER && !defined MSYS

#include "inputLinux.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

using namespace InputNS;

void InputLinux::changeMode(InputMode mode)
{
  if ( mode == INPUT_INTERACTIVE )
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else{
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
  }
}

int InputLinux::selectStdin()
  // check if input stream is readable, like windows _kbhit
  // change terminal mode first
{
  struct timeval tv;
  fd_set rdfs;

  tv.tv_sec = 0;
  tv.tv_usec = 0;

  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);

  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);

}

int InputLinux::getChin()
{
  return getchar();
}

/*int InputLinux::waitKey()
  // do everything for you
{
  int ch;
  changeMode(InputNS::INPUT_INTERACTIVE);
  while ( !kbhit() )
  {
    //putchar('.');
    //fflush(stdout);
    usleep(100000);
  }

  ch = getchar();

  changeMode(InputNS::INPUT_DEFAULT);
  return ch;
}*/

int InputLinux::getHardcoreKey(Hardcore &key)
{
  // TODO implement
  return 0;
}

int InputLinux::getModifiers()
{
  // TODO implement
  return 0;
}

// TODO deleteme
//int InputLinux::charToCode(char c)
//{
// return 0;
//}

int InputLinux::sendKeytoOS(char key)
{
  return 0;
}

int InputLinux::sendHardcoretoOS(Hardcore key)
{
  return 0;
}

#endif

