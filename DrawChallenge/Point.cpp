// Copyright (c) 2018 Art & Logic, Inc. All Rights Reserved.

#include "stdafx.h"
#include "Point.h"


APoint::APoint()
   : X(0)
   , Y(0)
{

}
APoint::APoint(SInt16 x, SInt16 y)
   : X(x)
   , Y(y)
{

}
APoint::~APoint()
{

}

// overload + operator to add two points together
APoint APoint::operator+(const APoint& p)
{
   APoint point;
   point.X = this->X + p.X;
   point.Y = this->Y + p.Y;
   return point;
}

APoint& APoint::operator+=(const APoint& p)
{
   this->X += p.X;
   this->Y += p.Y;
   return *this;
}
APoint APoint::operator-(const APoint& p)
{
   APoint point;
   point.X = this->X - p.X;
   point.Y = this->Y - p.Y;
   return point;

}
APoint APoint::operator*(const APoint& p)
{
   APoint point;
   point.X = this->X * p.X;
   point.Y = this->Y * p.Y;
   return point;
}

APoint APoint::operator*(float v)
{
   APoint point;
   point.X = (SInt16)(this->X * v + 0.4999999); // adding 0.499999 is just a cheap round up
   point.Y = (SInt16)(this->Y * v + 0.4999999);
   return point;
}
