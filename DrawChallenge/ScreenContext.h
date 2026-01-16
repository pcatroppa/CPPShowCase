// Copyright (c) 2018 Art & Logic, Inc. All Rights Reserved.
#pragma once

#ifndef h_ScreenContext
#define h_ScreenContext

#ifndef _WINDOWS
#define _WINDOWS
#include "AlTypes.h"
#endif

#include "Point.h"

#include <string>
#include <utility>

struct RGBColor
{
   UInt8 Red;
   UInt8 Green;
   UInt8 Blue;
   UInt8 Alpha;
};

struct Pen
{
   // X, Y position in a pair (for easy getting)
   APoint Pos;
   
   // Pen state can be up or down
   bool bDown;

   // Color
   RGBColor Color;
};

/**
* class AScreenContext
*
*  Screen context is a class that encapsulates a pen and screen
*
*  The main purpose of this class is to manage
*  drawing to a screen with a pen.
*/

class AScreenContext
{
public:
   AScreenContext();
   virtual ~AScreenContext();

   // clear screen contexts with notication
   void Clear();

   // Resets current position to 0,0 and moves pen up, resets to black
   void ResetPen();

   // Draw out our screen
   void DrawScreenContents();

   // Min/max screen coordinate getters
   SInt16 GetScreenMaxX() { return fMaxX; }
   SInt16 GetScreenMaxY() { return fMaxY; }
   SInt16 GetScreenMinX() { return fMinX; }
   SInt16 GetScreenMinY() { return fMinY; }

   // Pen up or down
   bool IsPenDown() { return fPen.bDown; }

   // returns true if the pen is down; false if the pen is up
   bool SetPenDown(bool newState);
   bool IsPenInBounds();

   // Move the pen (relative to current pos)
   void MovePenRelative(APoint distance);

   // The screen needs to handle a series of moves 
   void StartMove();
   void FinishMove();

   // Change color of the pen
   void SetPenColor(RGBColor& newColor);

   // Get the point at which two lines intersect (returns false if parallel or lines do not cross)
   bool GetPointOfIntersection(APoint p1, APoint p2, APoint& outputPoint);
   bool CheckLineIntersection(APoint A, APoint B, APoint C, APoint D, APoint& outputPoint);

protected:

   // Screen event notifications
   void OnNotifyScreenCleared();
   void OnNotifyPenChanged(bool bNewState);

   // Moves can be strung together and can be effected by screen edges
   void OnNotifyMoveStarted();
   void OnNotifyMoveExecuted(const Pen& pen);
   void OnNotifyMoveFinished(const Pen& pen);
   void OnNotifyEdgeHit(const APoint& edgePoint);

   void OnNotifyColorChanged(const RGBColor& newColor);

private:
   // Screen dimensions
   SInt16 fMaxX;
   SInt16 fMaxY;
   SInt16 fMinX;
   SInt16 fMinY;

   // The pen
   Pen fPen;

   // The screen (in this challenge it is just a string)
   std::string fScreenString;

   // Helper for when we kick the pen up
   bool bPenWantsDown;
};

#endif