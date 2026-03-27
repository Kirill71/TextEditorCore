#pragma once

/*---------------------------------------------------------------------------*/

#if defined(_WIN32) || defined(__CYGWIN__)
  #ifdef TEXTEDITORCORELIB_EXPORTS
    #define API __declspec(dllexport)
  #else
    #define API __declspec(dllimport)
  #endif
#else
  #define API __attribute__((visibility("default")))
#endif


/*---------------------------------------------------------------------------*/