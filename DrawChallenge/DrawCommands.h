// Copyright (c) 2018 Art & Logic, Inc. All Rights Reserved.
#pragma once

#ifndef h_DrawCommand
#define h_DrawCommand

#include "ScreenContext.h"
#include "LineValueDecoder.h"

/**
* class ADrawCommand
* Base class for the different drawing commands
* 
*  Used by clear, pen, move and color commands
*/
class ADrawCommand
{
public:
   ADrawCommand();
   virtual ~ADrawCommand();

   // Abstract pure virtual function must be implemented by each derived class
   virtual void ExecuteToScreen(class AScreenContext& screenContext) = 0;
};

/**
* class AParameterDrawCommand
* Derived from ADrawCommand and is also a base class 
* for the different drawing commands that have parameters
*
* Paramaters are encoded and a such need decoding
* LineValueDecoder will decode the parameters
*
*  Used by pen, move and color commands
*/
class AParameterDrawCommand : public ADrawCommand
{
public:
   AParameterDrawCommand(const std::string& encodedParams);

protected:
   // Will decode the values from the encodedParams
   ALineValueDecoder fParameter;
};


/**
* class AClearCommand
* Derived from ADrawCommand 
*
* Causes the drawing to reset itself.
*  - sets pen state to up
*  - sets pen pos to 0,0
*  - sets color to (0,0,0,255) (black)
*  - clears any output on the screen
*/
class AClearCommand : public ADrawCommand
{
public:
   AClearCommand();
   // handle clear command to screen
   virtual void ExecuteToScreen(class AScreenContext& screenContext);
};

/**
* class APenCommand
* Derived from AParameterDrawCommand 
* 
* Parameter can set pen up or down
*
*/
class APenCommand : public AParameterDrawCommand
{
public:
   APenCommand(const std::string& encodedParams);

   // Returns true if the pen is down.
   bool IsDown() { return bDown; }

   // handle pen up down output to screen
   virtual void ExecuteToScreen(class AScreenContext& screenContext);

private:
   bool bDown;
};

/**
* class AColorCommand
* Derived from AParameterDrawCommand
*
* Parameters can set the R G B and A (0..255)
*
*/
class AColorCommand : public AParameterDrawCommand
{
public:
   AColorCommand(const std::string& encodedParams);

   // handle color output
   virtual void ExecuteToScreen(class AScreenContext& screenContext);

private:
   RGBColor fColor;
};

/**
* class AMoveCommand
* Derived from AParameterDrawCommand
*
* Parameters will change the location of the pen
* can have multiple coordinates for each move command
*
*/
class AMoveCommand : public AParameterDrawCommand
{
public:
   AMoveCommand(const std::string& encodedParams);

   // handle mov output
   virtual void ExecuteToScreen(class AScreenContext& screenContext);

private:
   // Holds the decoded coordinates
   std::vector<APoint>fCoordinates;
};

#endif