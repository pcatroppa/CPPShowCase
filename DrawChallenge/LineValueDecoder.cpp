// Copyright (c) 2018 Art & Logic, Inc. All Rights Reserved.

#include "stdafx.h"
#include "LineValueDecoder.h"

#include <sstream>

static const SInt8 kEncodeLength = 4;

ALineValueDecoder::ALineValueDecoder()
{
}
ALineValueDecoder::ALineValueDecoder(const std::string& rawData)
{
   ProcessDataString(rawData);
}

ALineValueDecoder::~ALineValueDecoder()
{
}

void ALineValueDecoder::ProcessDataString(const std::string& encodedDataString)
{
   // We are given a raw string like so: 400000007F7F
   // convert it into the corresponding list of integer values
   // [0, -8192, 8191] 

   // split the string into X (kEncodeLength) chars at a time
   size_t pos = 0;
   size_t length = kEncodeLength;
   while (encodedDataString.length() >= pos+length) // we want hi and lo so 4 total
   {
      // Get next chunk of characters from the raw string
      std::string chunk = encodedDataString.substr(pos, length);
      
      // Decode and Add to our list
      SInt16 decodedValue=0;
      if (Decode(chunk, decodedValue))
      {
         fDecodedValues.push_back(decodedValue);
      }
      
      // move the length for next loop
      pos += length; 
   }
}

// Helper to decode a string (breaks up the hi and low)
bool ALineValueDecoder::Decode(const std::string& rawHiLow, SInt16& outputValue)
{
   // Here we should have a 4 character string
   if (rawHiLow.length() != 4)
      return false;

   UInt8 hi=0;
   if (!HexToUInt8(rawHiLow.substr(0, 2), hi))
   {
      return false;
   }
   UInt8 lo = 0;;
   if (!HexToUInt8(rawHiLow.substr(2, 2), lo))
   {
      return false;
   }

   outputValue = AValueEncoder::Decode(hi, lo);
   return true;
}


bool ALineValueDecoder::HexToUInt8(const std::string& hexString, UInt8& outputValue)
{
   // Convert hex to UInt8
   // First we need to use SInt16
   SInt16 tempValue;
   std::stringstream stream;
   stream << hexString;
   stream >> std::hex >> tempValue;

   // Now we have to check it here before casting
   if (tempValue < 0 || tempValue > 127)
   {
//#ifdef _DEBUG
//      throw std::invalid_argument("hex value is bigger than UIn8");
//#endif // _DEBUG
      return false;  
   }

   // It's ok cast it to our Uint8
   outputValue = (UInt8)tempValue;
   return true;
}

SInt16 ALineValueDecoder::GetDecodedItem(size_t index)
{
   // Check if we are in bounds
   if (index >= fDecodedValues.size())
   {
#ifdef _DEBUG
      throw std::invalid_argument("index is bigger than vector size");
#else
      return kInvalidItem;
#endif // _DEBUG
   }

   return fDecodedValues[index];
}
