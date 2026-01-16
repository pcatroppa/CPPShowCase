// Copyright (c) 2018 Art & Logic, Inc. All Rights Reserved.

#include "stdafx.h"
#include "ValueEncoder.h"
#include <sstream>

// constant used for encoding
static const UInt16 kAdjust = 8192;

AValueEncoder::AValueEncoder()
{
}

AValueEncoder::~AValueEncoder()
{
}

std::string AValueEncoder::Encode(UInt16 value)
{
	// The encoding process adds 8192 to the raw value, so its range is translated to [0..16383]
   // Then pack that value into two bytes such that the most significant bit of each is cleared.
   //Unencoded intermediate value (as a 16-bit integer):
      // 00HHHHHH HLLLLLLL 
   // Encoded value :
      // 0HHHHHHH 0LLLLLLL	 (!!! here you can see 0HHHHHHH is a result of 00HHHHHH left shifted by 1 !!!)
	// Format the two bytes as a single 4 char hex string and return it.


	// So first add constant 8192 to the value so now it is translated to [0..16383]
	value += kAdjust;

	// Next we are packing into two bytes (8 bits in a byte)
	// we need the binary or hex equivalent of 0 s in the correct bit for masking
	// so to toggle the bits for the high bits (and clear first bit with a 0) 0111111 00000000  
	UInt16 highBits = (UInt16)(value & 0x3F80);

	//  then to toggle the second bits (and clear first bit again) 00000000 01111111
	UInt16 lowBits = (UInt16)(value & 0x007F);

	// Next add the two bytes together but shift the highBits by 1
	UInt16 encodedValue = (UInt16)(lowBits + (highBits << 1));

	// Lastly we want to store a 4 character hex value.
	std::stringstream stream; // stringstream has a convenient hex format helper
	stream << std::hex << encodedValue;

	// Now return our hex result as a string
	std::string result(stream.str()); 
	return result;
}

SInt16 AValueEncoder::Decode(UInt8 highValue, UInt8 lowValue)
{
	// Recombine two bytes to return the corresponding integer between[-8192.. + 8191]
	// so we need to create a 16 bit from our two byte values 
	// say for exampe, highValue=00001100 and lowValue=00000011
	// we also need to restore our encoded left shift on the high value.
	// so the result would look like (00000110 00000011)

	// Create a 16 bit value for highValue (makes 00000000 00001100)
	UInt16 highBits = highValue; 
	// and restore the original high value by shifting left by 7
	highBits = (UInt16)(highBits << 7);
	// so now we should have (00000110 00000000)

	UInt16 lowBits = lowValue;
	UInt16 combinedBits = (UInt16)(lowBits + highBits); // simply adding em up becomes (00000110 00000011)

	// Lastly remove our constant to get our value between [-8192...+8191]
	SInt16 decodedValue = combinedBits - kAdjust;
	return decodedValue;
}

