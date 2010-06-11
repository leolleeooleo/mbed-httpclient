
/*
Copyright (c) 2010 Donatien Garnier (donatiengar [at] gmail [dot] com)
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

//#ifdef DBG_H
//#define DBG_H

#ifdef __LWIP_DEBUG
#define __DEBUG
#endif

#ifdef __DEBUG

#ifndef __DEBUGSTREAM
#define __DEBUGSTREAM


class DebugStream
{
public:
static void debug(const char* format, ...);
static void release();
private:

};

#undef DBG
#undef DBG_END
#define DBG DebugStream::debug
#define DBG_END DebugStream::release
#endif

#else
#undef DBG
#undef DBG_END
#define DBG(...)
#define DBG_END()
#endif

#ifdef __LWIP_DEBUG
#ifndef __SNPRINTF
#define __SNPRINTF
#include "mbed.h"

//int snprintf(char *str, int size, const char *format, ...);
#endif
#endif

#ifdef __LWIP_DEBUG
#undef __DEBUG
#endif

//#endif

