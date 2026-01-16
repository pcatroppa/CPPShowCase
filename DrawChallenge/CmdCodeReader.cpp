// Copyright (c) 2018 Art & Logic, Inc. All Rights Reserved.

#include "stdafx.h"
#include "CmdCodeReader.h"

#include "DrawCommands.h"

#include <sstream> // added for std::hex helper

static const SInt16 kCommandLength = 2; // commands come in as 2 chars (e.g. FA or A0)

static const std::string kClearCodeString = "F0";
static const std::string kPenCodeString   = "80";
static const std::string kColorCodeString = "A0";
static const std::string kMoveCodeString  = "C0";

ACmdCodeReader::ACmdCodeReader()
{
   fListOfCommands.clear();
}

ACmdCodeReader::ACmdCodeReader(const std::string& encodedDataString)
{
   ParseString(encodedDataString);
}

ACmdCodeReader::~ACmdCodeReader()
{
   // Clean up the commands
   for (ADrawCommand* drawCommand : fListOfCommands)
   {
      delete drawCommand;
      drawCommand = NULL;
   }
}

void ACmdCodeReader::ParseString(const std::string& encodedDataString)
{
   // In this system, commands are represented in the data stream by a single (un-encoded) 
   // opcode byte that can be identified by always having its most significant bit set, 
   // followed by zero or more bytes containing encoded data values. 
   // Any unrecognized commands encountered in an input stream should be ignored. 

   // e.g. |cmd|param|param|param|cmd|param|cmd|cmd|param|param|etc.

   SInt16 pos = 0;
   SInt16 length = kCommandLength;
   while (-1 != pos)
   {
      // Find the command
      std::string commandString;
      pos = FindNextCommand(pos, encodedDataString, commandString);
      if (-1 == pos)
         break;

      // Find the parameters for the command
      std::string params;
      pos = ParseCommandParams(pos + length, encodedDataString, params);

      // Create a commmand!
      CreateCommand(commandString, params);
   }
}


SInt16 ACmdCodeReader::FindNextCommand(SInt16 start, const std::string& encodedDataString, std::string& outPutCmdName)
{   
   // Returns the index of the next command given a starting point or -1

   // We know commands should be 2 chars  
   // however we can crawl 1 at time here for commands
   // (as they only have MSB set)
   // that way if there is garabage we still pickup at the next command

   SInt16 pos = start;
   SInt16 length = kCommandLength;
   while ((SInt16)encodedDataString.length() >= pos + length)
   {
      // Get next chunk of characters from the raw string
      std::string chunkofdata = encodedDataString.substr(pos, length);
      // Check for command
      if (IsCommandFound(chunkofdata))
      {
         outPutCmdName = chunkofdata;
         return pos;
      }
      // move pos for next loop
      pos += 1;   // length; 
   }

   return -1;
}

bool ACmdCodeReader::IsCommandFound(const std::string& checkString)
{
   // bounds check
   if (checkString.length() < kCommandLength || checkString.length() > kCommandLength)
      return false;

   //an opcode byte can be identified by always having its most significant bit set

   // convert to SInt16 and then do a check on the most significant bit
   SInt16 tempValue;
   std::stringstream stream;
   stream << checkString;
   stream >> std::hex >> tempValue;

   return ((tempValue & 0x80)>0);
}

SInt16 ACmdCodeReader::ParseCommandParams(SInt16 pos, const std::string& encodedDataString, std::string& foundParams)
{
   // Params are whatever is found between two commands (so get the chunk of data up til next command position)
   SInt16 length = kCommandLength;
   while ((SInt16)encodedDataString.length() >= pos + length)
   {
      // Get next chunk characters from the raw string
      std::string chunkofdata = encodedDataString.substr(pos, length);
      // Check for command
      if (IsCommandFound(chunkofdata))
         break;
      foundParams += chunkofdata;
      pos += length; // Now we are assuming our parameters are in order by 2 here... going by 1 breaks things as we get a false positive on a command
   }
   return pos;

}

void ACmdCodeReader::CreateCommand(const std::string& cmdOpcode, const std::string& params)
{
   // Create the corresponding Command 
   // based off the incoming cmdOpcode
   // ignoring unknown commands
   //
   // store them in the internal vector
   
   if (kClearCodeString == cmdOpcode)
   {
      AClearCommand* clearCommand = new AClearCommand();
      fListOfCommands.push_back(clearCommand);
   }
   else if (kPenCodeString == cmdOpcode)
   {
      APenCommand* penCommand = new APenCommand(params);
      fListOfCommands.push_back(penCommand);
   }
   else if (kColorCodeString == cmdOpcode)
   {
      AColorCommand* colorCommand = new AColorCommand(params);
      fListOfCommands.push_back(colorCommand);
   }
   else if (kMoveCodeString == cmdOpcode)
   {
      AMoveCommand* moveCommand = new AMoveCommand(params);
      fListOfCommands.push_back(moveCommand);
   }
}

ADrawCommand* ACmdCodeReader::GetDrawCommand(size_t item)
{
   // Check if we are in bounds
   if (item >= fListOfCommands.size())
   {
//#ifdef _DEBUG
//      throw std::invalid_argument("index is bigger than vector size");
//#endif // _DEBUG
      return NULL;
   }

   return fListOfCommands[item];
}
