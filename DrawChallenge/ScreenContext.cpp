// Copyright (c) 2018 Art & Logic, Inc. All Rights Reserved.

#include "stdafx.h"
#include "ScreenContext.h"

#include <iostream>
#include <algorithm>    // std::min

static const SInt16 kScreenMaxX = 8191;
static const SInt16 kScreenMaxY = 8191;
static const SInt16 kScreenMinX = -8192;
static const SInt16 kScreenMinY = -8192;

AScreenContext::AScreenContext()
   : fMaxX(kScreenMaxX)
   , fMaxY(kScreenMaxY)
   , fMinX(kScreenMinX)
   , fMinY(kScreenMinY)
{
   ResetPen();
}

AScreenContext::~AScreenContext()
{
}

void AScreenContext::Clear()
{
   //clear our screen
   fScreenString.empty();

   //reset the pen
   ResetPen();

   // Notificaiton
   OnNotifyScreenCleared();
}

void AScreenContext::ResetPen()
{
   fPen.bDown = false;
   fPen.Pos.X = 0;
   fPen.Pos.Y = 0;

   // reset color to black
   fPen.Color.Red = 0;
   fPen.Color.Green = 0;
   fPen.Color.Blue = 0;
   fPen.Color.Alpha = 255;

   bPenWantsDown = false;
}

void AScreenContext::SetPenColor(RGBColor& newColor)
{
   // Set the new color (check the bounds)
   if (newColor.Red >= 0 && newColor.Red < 256)
      fPen.Color.Red = newColor.Red;
   if (newColor.Green >= 0 && newColor.Green < 256)
      fPen.Color.Green = newColor.Green;
   if (newColor.Blue >= 0 && newColor.Blue < 256)
      fPen.Color.Blue = newColor.Blue;
   if (newColor.Alpha >= 0 && newColor.Alpha < 256)
      fPen.Color.Alpha = newColor.Alpha;

   OnNotifyColorChanged(newColor);
}

void AScreenContext::StartMove()
{
   OnNotifyMoveStarted();
}

void AScreenContext::FinishMove()
{
   OnNotifyMoveFinished(fPen);
}

void AScreenContext::MovePenRelative(APoint distance)
{
   // We have to handle moving the pen offscreen
   // it will kick the pen up (if it is down)
   // it will display (notify) the edge location (if pen down)
   // also (if pen down) when coming back on screen we need to draw at edge

   // hang on to our old pos and whether or not we are in bounds
   APoint oldPos = fPen.Pos;
   bool wasInBounds = IsPenInBounds();

   // Move the pen (relative to current pos)
   // even if we go off screen pen still accepts the movement
   fPen.Pos += distance;

   // Now check if we went out of bounds
   bool isInBounds = IsPenInBounds();

   // Figure out what to display to the user
   if (!wasInBounds && !isInBounds)
   {
      // do nothing
   }
   else if (wasInBounds && isInBounds)
   {
      // normal inbounds move
      OnNotifyMoveExecuted(fPen);
   }
   else if (wasInBounds && !isInBounds)
   {
      // went out of bounds - kick the pen up!
      if (fPen.bDown)
      {
         bPenWantsDown = true; // remember it was down

         // Get the last draw point
         APoint edgePoint;
         GetPointOfIntersection(oldPos, fPen.Pos, edgePoint);
         OnNotifyEdgeHit(edgePoint); // !!! PAUSES the move

         SetPenDown(false);

         StartMove();               // !!! restart/unpause the move
      }
   }
   else if (!wasInBounds && isInBounds)
   {
      // came back in bounds
      // kick the back pen down, only if we kicked the pen up!
      if (!fPen.bDown && bPenWantsDown)
      {
         bPenWantsDown = false;
         APoint edgePoint;
         GetPointOfIntersection(oldPos, fPen.Pos, edgePoint);
         OnNotifyEdgeHit(edgePoint);   // !!! Pauses the move

         SetPenDown(true);

         StartMove();                  // !!! resume the move 

         OnNotifyMoveExecuted(fPen);
      }
   }
}

bool AScreenContext::SetPenDown(bool newState)
{
   bool oldState = fPen.bDown;

   // Pen can only go down if it is inside screen coords
   if (newState && IsPenInBounds())
      fPen.bDown = true;
   else
      fPen.bDown = false; 

   // Notification only if pen state changed
   if (oldState != fPen.bDown)
   {
      OnNotifyPenChanged(fPen.bDown);
   }

   return fPen.bDown;
}

bool AScreenContext::IsPenInBounds()
{
   // check if we are out of bounds
   if (fPen.Pos.X < fMinX || fPen.Pos.X > fMaxX
      || fPen.Pos.Y < fMinY || fPen.Pos.Y > fMaxY)
   {
         return false;
   }
   return true;
}

bool AScreenContext::GetPointOfIntersection(APoint p1, APoint p2, APoint& intersectPoint)
{
   APoint topLeft(GetScreenMinX(), GetScreenMinY());
   APoint topRight(GetScreenMaxX(), GetScreenMinY());
   APoint bottomLeft(GetScreenMinX(), GetScreenMaxY());
   APoint bottomRight(GetScreenMaxX(), GetScreenMaxY());

   // Check the top edge
   if (CheckLineIntersection(p1, p2, topLeft, topRight, intersectPoint))
   {
      return true;
   }
   // Check the right edge
   if (CheckLineIntersection(p1, p2, topRight, bottomRight, intersectPoint))
   {
      return true;
   }
   // Check the bottom edge
   if (CheckLineIntersection(p1, p2, bottomLeft, bottomRight, intersectPoint))
   {
      return true;
   }
   // Check the left edge
   if (CheckLineIntersection(p1, p2, topLeft, bottomLeft, intersectPoint))
   {
      return true;
   }

   return false;
}

bool AScreenContext::CheckLineIntersection(APoint p1, APoint p2, APoint p3, APoint p4, APoint& outputPoint)
{
   // Paul - I hope it is ok that I borrowed some code
   // http://flassari.is/2008/11/line-line-intersection-in-cplusplus/

   // Store the values for fast access and easy
   // equations-to-code conversion
   float x1 = p1.X, x2 = p2.X, x3 = p3.X, x4 = p4.X;
   float y1 = p1.Y, y2 = p2.Y, y3 = p3.Y, y4 = p4.Y;

   float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
   // If d is zero, there is no intersection
   if (d == 0) return false;

   // Get the x and y
   float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
   float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
   float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

   // Check if the x and y coordinates are within both lines
   if (x < std::min(x1, x2) || x > std::max(x1, x2) ||
      x < std::min(x3, x4) || x > std::max(x3, x4)) return false;
   if (y < std::min(y1, y2) || y > std::max(y1, y2) ||
      y < std::min(y3, y4) || y > std::max(y3, y4)) return false;

   // Output the point of intersection
   outputPoint.X = (SInt16)std::round(x);
   outputPoint.Y = (SInt16)std::round(y);
   return true;
}

void AScreenContext::OnNotifyScreenCleared() 
{
   fScreenString += "CLR;\n";
}

void AScreenContext::OnNotifyPenChanged(bool bNewDownState)
{
   if (bNewDownState)
      fScreenString += "PEN DOWN;\n";
   else
      fScreenString += "PEN UP;\n";
}

void AScreenContext::OnNotifyMoveStarted()
{
   fScreenString += "MV";
}

void AScreenContext::OnNotifyMoveExecuted(const Pen& pen)
{
   // only moves with pen down are displayed
   if (pen.bDown)
   {
      fScreenString += " (";
      fScreenString += std::to_string(pen.Pos.X);
      fScreenString += ", ";
      fScreenString += std::to_string(pen.Pos.Y);
      fScreenString += ")";
   }
}

void AScreenContext::OnNotifyMoveFinished(const Pen& pen)
{
   if (!pen.bDown)
   {
      // When our pen is up we want to know where it left off.
      fScreenString += " (";
      fScreenString += std::to_string(pen.Pos.X);
      fScreenString += ", ";
      fScreenString += std::to_string(pen.Pos.Y);
      fScreenString += ")";
   }

   fScreenString += ";\n";
}

void AScreenContext::OnNotifyEdgeHit(const APoint& edgePoint)
{
   fScreenString += " (";
   fScreenString += std::to_string(edgePoint.X);
   fScreenString += ", ";
   fScreenString += std::to_string(edgePoint.Y);
   fScreenString += ")";

   //!!! always finishes the move
   fScreenString += ";\n";
}

void AScreenContext::OnNotifyColorChanged(const RGBColor& newColor)
{
   fScreenString += "CO ";
   fScreenString += std::to_string(newColor.Red);
   fScreenString += " ";
   fScreenString += std::to_string(newColor.Green);
   fScreenString += " ";
   fScreenString += std::to_string(newColor.Blue);
   fScreenString += " ";
   fScreenString += std::to_string(newColor.Alpha);
   fScreenString += ";\n";
}

void AScreenContext::DrawScreenContents()
{
   // Draw out our screen (just a string for this challenge)
   std::cout << fScreenString << "\n";
}
