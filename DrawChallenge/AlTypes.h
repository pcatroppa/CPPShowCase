#pragma once
#ifndef h_AlTypes
#define h_AlTypes

#ifdef _WINDOWS
// Microsoft Visual C++ Version
typedef char               SInt8;
typedef unsigned char      UInt8;
typedef short              SInt16;
typedef unsigned short     UInt16;
typedef long               SInt32;
typedef unsigned long      UInt32;
typedef __int64            SInt64;
typedef unsigned __int64   UInt64;

#else // Macintosh version
#ifndef __MACTYPES__
//typedef char SInt8;
typedef unsigned char UInt8;
typedef short SInt16;
typedef unsigned short UInt16;
typedef long SInt32;
typedef unsigned long UInt32;
typedef long long SInt64;
typedef unsigned long long UInt64;
#endif

#endif

#endif  /* This must be the final line in this file. */
