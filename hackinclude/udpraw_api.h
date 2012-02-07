#ifndef UDPRAW_API_H
#define UDPRAW_API_H

// set up windows dll stuff
#ifdef _WIN32
  #ifdef UDPRAW_EXPORTS // define this when compiling the dll, otherwise don't!
    #define UDPRAW_API __declspec(dllexport)
  #else
    #define UDPRAW_API __declspec(dllimport)
  #endif
#else
  // define api to be nothing for linux
  #define UDPRAW_API
#endif

#endif
