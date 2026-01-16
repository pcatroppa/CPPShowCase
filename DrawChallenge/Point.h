// Copyright (c) 2018 Art & Logic, Inc. All Rights Reserved.
#pragma once

#ifndef h_Point
#define h_Point

#ifndef _WINDOWS
#define _WINDOWS
#include "AlTypes.h"
#endif

/**
* class APoint
*
* A class to encapslute screen X and Y points
* overloaded oeprators allow for adding
* subtracting and multipling two points
*/
class APoint
{
public:
   APoint();
   APoint(SInt16 p1, SInt16 p2);
   virtual ~APoint();

   // overload + operator to add two points together
   APoint operator+(const APoint& p);
   APoint& operator+=(const APoint& p);
   APoint operator-(const APoint& p);
   APoint operator*(const APoint& p);
   APoint operator*(float v);

   // Public members usually a no-no but for a point system, it's ok.
   SInt16 X;
   SInt16 Y;
};

#endif