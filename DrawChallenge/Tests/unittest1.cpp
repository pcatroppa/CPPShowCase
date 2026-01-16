#include "stdafx.h"
#include "CppUnitTest.h"
#include "../LineValueDecoder.h"
#include "../CmdCodeReader.h"
#include "../ScreenContext.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethodEncode)
		{
         Assert::IsTrue("4000" == AValueEncoder::Encode(0));
         Assert::IsTrue("0" == AValueEncoder::Encode(-8192));
         Assert::IsTrue("7f7f" == AValueEncoder::Encode(8191));
         Assert::IsTrue("5000" == AValueEncoder::Encode(2048));
         Assert::IsTrue("2000" == AValueEncoder::Encode(-4096));
      }
      TEST_METHOD(TestMethodDecode)
      {
         Assert::IsTrue((0 == AValueEncoder::Decode(0x40, 0x00)));
         Assert::IsTrue((-8192 == AValueEncoder::Decode(0x00, 0x00)));
         Assert::IsTrue((8191 == AValueEncoder::Decode(0x7F, 0x7F)));
         Assert::IsTrue((2048 == AValueEncoder::Decode(0x50, 0x00)));
         Assert::IsTrue((-6907 == AValueEncoder::Decode(0x0A, 0x05)));
         Assert::IsTrue((2688 == AValueEncoder::Decode(0x55, 0x00)));
      }
      TEST_METHOD(TestMethodLineValueDecode)
      {
         ALineValueDecoder lineValue("400000007F7F");
         Assert::IsTrue((0 == lineValue.GetDecodedItem(0)));
         Assert::IsTrue((-8192 == lineValue.GetDecodedItem(1)));
         Assert::IsTrue((8191 == lineValue.GetDecodedItem(2)));

         // Should be 3 items
         Assert::IsTrue((3 == lineValue.CountOfDecodedItems()));
      }
      TEST_METHOD(TestMethodLineValueDecodeHiLow)
      {
         SInt16 outvalue;
         Assert::IsTrue(ALineValueDecoder::Decode("4000", outvalue));
         Assert::IsTrue((AValueEncoder::Decode(0x40, 0x00) == outvalue));
      }
      TEST_METHOD(TestMethodHexToUint8)
      {
         UInt8 outvalue;
         Assert::IsTrue(ALineValueDecoder::HexToUInt8("00",outvalue));
         Assert::IsTrue(0 == outvalue);
         Assert::IsTrue(ALineValueDecoder::HexToUInt8("7F", outvalue));
         Assert::IsTrue(127 == outvalue);
         Assert::IsTrue(ALineValueDecoder::HexToUInt8("63", outvalue));
         Assert::IsTrue(99 == outvalue);
         Assert::IsFalse(ALineValueDecoder::HexToUInt8("F6F3", outvalue));
      }
      TEST_METHOD(TestMethodGetDecodedItem)
      {
         ALineValueDecoder lineValue("400000007F7F");
         // Should be 3 items
         Assert::IsTrue((3 == lineValue.CountOfDecodedItems()));

#ifdef _DEBUG
         bool bAsserts = false;
         try
         {
            lineValue.GetDecodedItem(lineValue.CountOfDecodedItems());
         }
         catch (std::invalid_argument& ex)
         {
            ex;
            bAsserts = true;
         }
         Assert::IsTrue(bAsserts);
#else
         Assert::IsTrue(kInvalidItem == lineValue.GetDecodedItem(lineValue.CountOfDecodedItems()));

#endif
      }

      TEST_METHOD(TestMethodIsCommandFound)
      {
         ACmdCodeReader reader; 
         Assert::IsTrue((0 == reader.IsCommandFound("00")));
         Assert::IsTrue((1 == reader.IsCommandFound("F0")));
         Assert::IsTrue((1 == reader.IsCommandFound("A0")));
         Assert::IsTrue((1 == reader.IsCommandFound("C0")));
         Assert::IsTrue((0 == reader.IsCommandFound("04")));
         Assert::IsTrue((0 == reader.IsCommandFound("14")));
         Assert::IsTrue((0 == reader.IsCommandFound("00010101010101")));
      }
      TEST_METHOD(TestMethodFindNextCommand)
      {
         ACmdCodeReader reader;
         std::string outname;
         Assert::IsTrue((-1 == reader.FindNextCommand(0,"00", outname)));
         Assert::IsTrue((-1 == reader.FindNextCommand(0,"00000001000", outname)));
         Assert::IsTrue((0 == reader.FindNextCommand(0, "A0", outname)));
         Assert::IsTrue((0 == reader.FindNextCommand(0, "C0", outname)));
         Assert::IsTrue((-1 == reader.FindNextCommand(0, "04", outname)));
         Assert::IsTrue((2 == reader.FindNextCommand(0, "14A0", outname)));
         Assert::IsTrue((0 == reader.FindNextCommand(0, "abcdefgh", outname)));
      }
      TEST_METHOD(TestMethodParseCommandParams)
      {
         ACmdCodeReader reader;
         std::string outparams;
         Assert::IsTrue((2 == reader.ParseCommandParams(0, "00", outparams)));
         Assert::IsTrue((10 == reader.ParseCommandParams(0, "00000001000", outparams)));
         Assert::IsTrue((0 == reader.ParseCommandParams(0, "A0", outparams)));
         Assert::IsTrue((0 == reader.ParseCommandParams(0, "C0", outparams)));
         Assert::IsTrue((2 == reader.ParseCommandParams(0, "04", outparams)));
         Assert::IsTrue((2 == reader.ParseCommandParams(0, "14A0", outparams)));
         Assert::IsTrue((0 == reader.ParseCommandParams(0, "", outparams)));
         Assert::IsTrue((4 == reader.ParseCommandParams(4, "", outparams)));
         Assert::IsTrue((4 == reader.ParseCommandParams(4, "abcdefgh", outparams)));
      }
      TEST_METHOD(TestMethodIsPenInBounds)
      {
         AScreenContext screen;
         screen.MovePenRelative(APoint(1,1));
         Assert::IsTrue(screen.IsPenInBounds());
         screen.MovePenRelative(APoint(-10, -10));
         Assert::IsTrue(screen.IsPenInBounds());
         screen.MovePenRelative(APoint(111, -111));
         Assert::IsTrue(screen.IsPenInBounds());
         screen.MovePenRelative(APoint(-222, 222));
         Assert::IsTrue(screen.IsPenInBounds());
         screen.MovePenRelative(APoint(8191, 8191));
         Assert::IsFalse(screen.IsPenInBounds());

         screen.Clear();
         Assert::IsTrue(screen.IsPenInBounds());
         screen.MovePenRelative(APoint(8191, 8191));
         Assert::IsTrue(screen.IsPenInBounds());
         screen.MovePenRelative(APoint(1, 1));
         Assert::IsFalse(screen.IsPenInBounds());
         screen.MovePenRelative(APoint(-1, -1));
         Assert::IsTrue(screen.IsPenInBounds());

         screen.Clear();
         screen.MovePenRelative(APoint(-8192, -8192));
         Assert::IsTrue(screen.IsPenInBounds());
         screen.MovePenRelative(APoint(-1, 1));
         Assert::IsFalse(screen.IsPenInBounds());
         screen.MovePenRelative(APoint(1, 0));
         Assert::IsTrue(screen.IsPenInBounds());

         screen.Clear();
         screen.MovePenRelative(APoint(screen.GetScreenMaxX()+1, screen.GetScreenMaxY()));
         Assert::IsFalse(screen.IsPenInBounds());
         screen.Clear();
         screen.MovePenRelative(APoint(screen.GetScreenMaxX(), screen.GetScreenMaxY()+1));
         Assert::IsFalse(screen.IsPenInBounds());
         screen.Clear();
         screen.MovePenRelative(APoint(-8193, -8192));
         Assert::IsFalse(screen.IsPenInBounds());
         screen.Clear();
         screen.MovePenRelative(APoint(-8192, -8193));
         Assert::IsFalse(screen.IsPenInBounds());
         screen.MovePenRelative(APoint(screen.GetScreenMinX() - 1, screen.GetScreenMinY()));
         Assert::IsFalse(screen.IsPenInBounds());
         screen.Clear();
         screen.MovePenRelative(APoint(screen.GetScreenMinX(), screen.GetScreenMinY() - 1));
         Assert::IsFalse(screen.IsPenInBounds());
      }
	};
}