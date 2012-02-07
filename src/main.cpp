#include <cstdlib>
#include "getopt_basic.h"
#include <iostream>
#ifdef _WIN32
#include "inputWin.h"
#else
#include "inputLinux.h"
#endif
#include <string>
#include "udpraw.h"

using namespace std;

namespace netkey {
  enum Mode { MODE_SEND, MODE_RECV };

  struct Args {
    Mode mode;
    string ip;
    int port;
    Args() : mode(MODE_SEND), ip("127.0.0.1"), port(9898) {}
  };

}

using namespace netkey;


void beSender(Args &args)
{
  Udpraw udp;
  udp.initSender(args.ip.c_str(), args.port);

  Input *input;
#ifdef _WIN32
  input = new InputWin;
#else
  input = new InputLinux;
#endif

  char buf[2];
  bool end = false;
  int chin = 0; // character in

  cout << "Enter key to send, Q to quit." << endl;

  while (!end) {
    chin = input->waitKey(); // wait for keypress

    if (chin != 'Q') {

      buf[0] = chin;
      buf[1] = '\0';
      udp.sendRaw(buf, 2, false);

    }else end = true;
  }

  udp.closeAndCleanup();
}

void beReceiver(Args &args)
{
  Udpraw udp;
  udp.initReceiver(args.port);

  Input *input;
#ifdef _WIN32
  input = new InputWin;
#else
  input = new InputLinux;
#endif

  int bytesRecvd = 0;
  char buf[2];
  string str;

  while (1) {
    bytesRecvd = udp.recvRaw(buf, 2, false);
    if (bytesRecvd < 1) udp.writeError();
    else {
      buf[1] = '\0'; // just to be sure
      cout << "Writing: " << buf << endl;
      int temp = (int) buf[0];
      cout << "as int: " << temp << endl;

      //str = "autoit3 /AutoIt3ExecuteLine \"Send('";
      //str += buf;
      //str += "')\"";
      //system(str.c_str());

      cout << "Sleeping" << endl;
      Sleep(2000);
      cout << "Sending now" << endl;
      input->sendKeytoOS(buf[0]);

    }
  }

  udp.closeAndCleanup();
}


void usage(Args &args)
{
  cout << "Usage:\n"
       << "  netkey [options]\n\n"
       << "Options:\n"
       << "  -h  --help\n"
       << "    Display this info\n"
       << "  -i  --ipaddress\n"
       << "    IP address to use for sending (currently: " << args.ip << ")\n"
       << "  -p  --port\n"
       << "    Port to use for sending or receiving (currently: " << args.port << ")\n"
       << "  -r  --recv\n"
       << "    Receive key\n"
       << "  -s  --send\n"
       << "    Send key (default)\n"
       << "Example:\n"
       << "  netkey\n"
       << "  netkey -r" << endl;
}

void processCLAs(int argc, char** argv, Args &args)
{
  namespace gob = getopt_basic;

  struct gob::option options[] = {
    {"help", no_argument, 0, 'h'},
    {"ipaddress", required_argument, 0, 'i'},
    {"port", required_argument, 0, 'p'},
    {"recv", no_argument, 0, 'r'},
    {"send", no_argument, 0, 's'},
    {0, 0, 0, 0}
  };

  int idx = 0;
  int c = 0;
  
  while ((c = gob::getopt_basic(argc, argv, "hi:p:rs", options, &idx)) != -1) {
    switch (c) {
      case 'h':
        usage(args);
        exit(0);
        break;
      case 'i':
        args.ip = gob::optarg;
        break;
      case 'p':
        args.port = atoi(gob::optarg);
        break;
      case 'r':
        args.mode = MODE_RECV;
        break;
      case 's':
        args.mode = MODE_SEND;
        break;
    }
  }

  // output what we're using

  cout << "Mode: ";
  if (args.mode == MODE_SEND) cout << "send\nIP: " << args.ip; else cout << "recv";
  cout << "\nPort: " << args.port << endl;

}

int main(int argc, char** argv)
{
  Args args;
  processCLAs(argc, argv, args);

  if (args.mode == MODE_SEND) beSender(args);
  else beReceiver(args);

  return 0;
}

