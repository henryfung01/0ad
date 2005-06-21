// compatibility fixes when compiling on Win32
// Copyright (c) 2002-2005 Jan Wassenberg
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// Contact info:
//   Jan.Wassenberg@stud.uni-karlsruhe.de
//   http://www.stud.uni-karlsruhe.de/~urkt/

#ifndef WIN_H__
#define WIN_H__

#ifndef _WIN32
#error "including win.h without _WIN32 defined"
#endif

#include "config.h"

#include <wchar.h>

// provide C99 *snprintf functions if compiler doesn't already
// (MinGW does, VC7.1 doesn't).
#ifndef HAVE_C99
# define snprintf _snprintf
# define swprintf _snwprintf
# define vsnprintf _vsnprintf
# define vsnwprintf _vsnwprintf
#endif

#include <stddef.h>	// wchar_t

// libpng.h -> zlib.h -> zconf.h includes <windows.h>, which causes conflicts.
// prevent that, and define what they actually need from windows.h.
// incidentally, this requires all dependents of windows.h to include
// sysdep/win/win_internal.h instead.
#define _WINDOWS_	// windows.h include guard
#define WINAPI __stdcall
#define WINAPIV __cdecl


//
// main() hook
//

// our Windows-specific init code needs to run before the regular main() code.
// ideally this would happen automagically, but there are two problems:
// - the C standard expressly forbids calling main() directly;
//   VC apparently makes use of this and changes its calling convention.
//   if we call it, everything appears to work but stack traces in
//   release mode are incorrect (symbol address is off by 4).
// - other libraries may also want to hook main(); in that case,
//   "one must fall". we need to provide for disabling our hook.
//   this is not enough reason to forego a hook entirely -
//   integration into new projects is easier when there is less
//   stuff to remember (here, calling our init function directly).
//
// what we'll do is: redefine the app's main function to app_main, have the
// OS call our main, and call app_main from there. in case another library
// (e.g. SDL) has the same idea, #define NO_MAIN_REDIRECT prevents the
// above; you then need to call win_pre_main_init at the beginning of main().
//#define NO_MAIN_REDIRECT

#ifdef NO_MAIN_REDIRECT
extern void win_pre_main_init();
#else
#define main app_main
#endif

#endif	// #ifndef WIN_H__
