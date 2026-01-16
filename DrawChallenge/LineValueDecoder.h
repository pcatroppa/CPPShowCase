// Copyright (c) 2018 Art & Logic, Inc. All Rights Reserved.
#pragma once

#ifndef h_LineValueDecoder
#define h_LineValueDecoder

#include "ValueEncoder.h"
#include <vector>

static const SInt16 kInvalidItem = 32767;

/**
* class ALineValueDecoder
* (extends the functionality of AValueEncoder)
*
* For decoding a line (or string) of Values
* Accepts a string as paramater
* Holds the List of integers decoded from the String .
*/

class ALineValueDecoder : public AValueEncoder
{
public:
	/**
	* Construct and initialize the LineValueDecoder object.
   * \param value - as our string of data to decode
	*/
   ALineValueDecoder();
   ALineValueDecoder(const std::string& encodedDataString);

	/**
	* Destroy the object, performing any needed cleanup.
	*/
	virtual ~ALineValueDecoder();

   // Returns count of decoded values
   UInt16 CountOfDecodedItems() { return (UInt16)fDecodedValues.size(); }

   // Get decoded value at a given index
   SInt16 GetDecodedItem(size_t index);

   // Helper to decode a string (breaks up the hi and low)
   static bool Decode(const std::string& rawHiLow, SInt16& outputValue);

   // Helper to convert Hex to Uint8
   static bool HexToUInt8(const std::string& hexString, UInt8& outputValue);

   // Do the work
   void ProcessDataString(const std::string& encodedDataString);

private:

	// Holds our decoded list of integer values
   std::vector<SInt16> fDecodedValues; 
};

#endif