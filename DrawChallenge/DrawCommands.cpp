// Copyright (c) 2018 Art & Logic, Inc. All Rights Reserved.

#include "stdafx.h"
#include "DrawCommands.h"

#include <utility>

ADrawCommand::ADrawCommand()
{
}
ADrawCommand::~ADrawCommand()
{
}
AParameterDrawCommand::AParameterDrawCommand(const std::string& params)
{
   // Parameters for commands will get processed here
   fParameter.ProcessDataString(params);
}

AClearCommand::AClearCommand()
{
}

void AClearCommand::ExecuteToScreen(class AScreenContext& screenContext)
{
   screenContext.Clear();
}

AMoveCommand::AMoveCommand(const std::string& params) : AParameterDrawCommand(params)
{  
   // fParameter will contain the decoded coordinates
   for (SInt16 index = 0; index < fParameter.CountOfDecodedItems(); ++index)
   {
      // Pair up the coordinates
      if (fParameter.CountOfDecodedItems() > index + 1)
      {
         //std::pair<SInt16, SInt16> coordinate;
         APoint coordinate;
         coordinate.X = fParameter.GetDecodedItem(index);
         coordinate.Y = fParameter.GetDecodedItem(++index);
         fCoordinates.push_back(coordinate);
      }
   }
}

void AMoveCommand::ExecuteToScreen(class AScreenContext& screenContext)
{
   // Tell the screen we are going to send move commands now...
   screenContext.StartMove();
   // Loop through all the moves we have for this command
   for (SInt16 index = 0; index < (SInt16)fCoordinates.size(); ++index)
   {
      // Do each move
      screenContext.MovePenRelative(fCoordinates[index]);
   }
   // Tell the screen we are done with our moves
   screenContext.FinishMove();
}


APenCommand::APenCommand(const std::string& params) : AParameterDrawCommand(params)
{  
   // fParameter will contain the decoded up or down value
   if (fParameter.CountOfDecodedItems() > 0)
      bDown = (fParameter.GetDecodedItem(0) > 0);
}

void APenCommand::ExecuteToScreen(class AScreenContext& screenContext)
{
   screenContext.SetPenDown(this->IsDown());
}

AColorCommand::AColorCommand(const std::string& params) : AParameterDrawCommand(params)
{
   // fParameter will contain the decoded color values 
   if (fParameter.CountOfDecodedItems() > 3) // must have 4 values 
   {
      fColor.Red = (UInt8)fParameter.GetDecodedItem(0);
      fColor.Green = (UInt8)fParameter.GetDecodedItem(1);
      fColor.Blue = (UInt8)fParameter.GetDecodedItem(2);
      fColor.Alpha = (UInt8)fParameter.GetDecodedItem(3);
   }
}

void AColorCommand::ExecuteToScreen(class AScreenContext& screenContext)
{
   screenContext.SetPenColor(fColor);
}



