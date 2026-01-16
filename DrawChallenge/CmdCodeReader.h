// Copyright (c) 2018 Art & Logic, Inc. All Rights Reserved.
#pragma once

#ifndef h_CmdCodeReader
#define h_CmdCodeReader

#ifndef _WINDOWS
#define _WINDOWS
#include "AlTypes.h"
#endif

#include <string>
#include <vector>

/**
* class ACmdCodeReader
* For reading codes from raw data
* Parse - read a raw data string and parse into individual codes
* 
*/

class ACmdCodeReader
{
public:
	/**
	* Construct and initialize the Encoder object.
	*/
   ACmdCodeReader();
   ACmdCodeReader(const std::string& encodedDataString);

	/**
	* Destroy the ACmdCodeReader object, performing any needed cleanup.
	*/
	virtual ~ACmdCodeReader();

   void ParseString(const std::string& encodedDataString);

   // returns the index of the next command given a starting point
   SInt16 FindNextCommand(SInt16 start, const std::string& encodedDataString, std::string& outPutCmdName);

   // returns the pos of the end of, outputs params into foundParams
   SInt16 ParseCommandParams(SInt16 pos, const std::string& encodedDataString, std::string& foundParams);

   // Create the command from a found command
   void CreateCommand(const std::string& cmdOpcode, const std::string& params);

   // helper to check the value for a most signficant bit set
   virtual bool IsCommandFound(const std::string& checkString);

   // Count of commands
   SInt16 CountOfCommands() { return (SInt16)fListOfCommands.size(); }

   // Get a command at index
   class ADrawCommand* GetDrawCommand(size_t item);

private:
   std::vector<class ADrawCommand* > fListOfCommands;

};

#endif