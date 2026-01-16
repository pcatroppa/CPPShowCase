// Copyright (c) 2018 Art & Logic, Inc. All Rights Reserved.
#pragma once

#ifndef h_ValueEncoder
#define h_ValueEncoder

#ifndef _WINDOWS
#define _WINDOWS
#include "AlTypes.h"
#endif
#include <string>

/**
* class AValueEncoder
* For Encoding and Decoding  Values
* Encode - Can convert an Integer into a special text encoding
* Decode - Can conver the endcoded value back into the original integer.
*/

class AValueEncoder
{
public:
	/**
	* Construct and initialize the Encoder object.
	*/
	AValueEncoder();

	/**
	* Destroy the Encoder object, performing any needed cleanup.
	*/
	virtual ~AValueEncoder();

	// Encode the value - returns an encoded hex string
	static std::string Encode(UInt16 value);

	// Decode two hex values - returns a decoded integer value
   static SInt16 Decode(UInt8 highValue, UInt8 lowValue);

};

#endif