// DrawChallenge.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DrawChallenge.h"

#include "CmdCodeReader.h"
#include "DrawCommands.h"
#include "ScreenContext.h"

#include <iostream>
#include <algorithm> // remove_if - white spaces
#include <cctype>    // isalnum

int main()
{
   ADrawChallenge drawChallenge;
   drawChallenge.LoopUserInterface();
   return 0;
}

ADrawChallenge::ADrawChallenge()
   : bCleanInput(false)
{
}

void ADrawChallenge::LoopUserInterface()
{
   // Loop until user quits
   while (1)
   {
      // Get raw data from user
      std::string input;
      std::cout << "*** Enter Encoded Data (q - quit, t - test, x - extended tests, c - toggle cleaning):\n";
      std::getline(std::cin, input);

      // check for quit
      if ("q" == input || "Q" == input)
      {
         break;
      }
      // check for test
      if ("t" == input || "T" == input)
      {
         RunBasicTests();
         continue;
      }
      // check for extended test
      if ("x" == input || "X" == input)
      {
         RunAllTests();
         continue;
      }
      // check for white space toggle
      if ("c" == input || "c" == input)
      {
         bCleanInput = !bCleanInput;
         if (bCleanInput)
         {
            std::cout << "White Spaces and non-alpha numeric values will be removed.\n";
         }
         else
         {
            std::cout << "No cleaning will be performed.\n";
         }
         continue;
      }

      // remove white spaces and such?
      if (bCleanInput)
      {
         CleanInput(input);
      }

      // Run the user encoded command
      RunInput(input);
   }
}

void ADrawChallenge::RunInput(const std::string& commandInput)
{
   // Read in the codes
   ACmdCodeReader parser(commandInput);
   if (parser.CountOfCommands() < 1)
   {
      std::cout << "No Commands Found!\n\n";
      return;
   }

   // Create a screen
   AScreenContext theScreen;

   // Execute the commands to the screen
   for (SInt16 index = 0; index < parser.CountOfCommands(); ++index)
   {
      ADrawCommand* pCommand = parser.GetDrawCommand(index);
      pCommand->ExecuteToScreen(theScreen);
   }

   // Draw the screen
   theScreen.DrawScreenContents();
}

void ADrawChallenge::CleanInput(std::string& rawInput)
{
   // white spaces
   rawInput.erase(std::remove_if(rawInput.begin(), rawInput.end(), isspace), rawInput.end());

   // non-alpha numeric
   rawInput.erase(std::remove_if(rawInput.begin(), rawInput.end(), [](char c) { return !std::isalnum(c); }), rawInput.end());
}

void ADrawChallenge::RunBasicTests()
{
   RunTest1();
   RunTest2();
   RunTest3();
   RunTest4();
}
void ADrawChallenge::RunAllTests()
{
   std::cout << "Running All Tests...(ignores cleaning option)\n\n";
   RunBasicTests();
   RunTest5();
   RunTest6();
   RunTest7();
   RunTest8();
   RunTest9();
   RunTestDrawP();
}

void ADrawChallenge::RunTest1()
{
   std::string testCode = "F0A04000417F4000417FC040004000804001C05F205F20804000";
   std::cout << "TEST 1: Green Line Simple...\nCODE: " << testCode << "\n\n";
   RunInput(testCode);
}
void ADrawChallenge::RunTest2()
{
   std::string testCode = "F0A040004000417F417FC04000400090400047684F5057384000804001C05F204000400001400140400040007E405B2C4000804000";
   std::cout << "TEST 2: Blue Square...\nCODE: " << testCode << "\n\n";
   RunInput(testCode);
}
void ADrawChallenge::RunTest3()
{
   std::string testCode = "F0A0417F40004000417FC067086708804001C0670840004000187818784000804000";
   std::cout << "TEST 3: Red clipping...\nCODE: " << testCode << "\n\n";
   RunInput(testCode);
}
void ADrawChallenge::RunTest4()
{
   std::string testCode = "F0A0417F41004000417FC067086708804001C067082C3C18782C3C804000";
   std::cout << "TEST 4: Orange angle clipping...\nCODE: " << testCode << "\n\n";
   RunInput(testCode);
}

void ADrawChallenge::RunTest5()
{
   std::string testCode = "F0A04000417F4000417FC040004000804001C05F205F20804000F0A040004000417F417FC04000400090400047684F5057384000804001C05F204000400001400140400040007E405B2C4000804000F0A0417F40004000417FC067086708804001C0670840004000187818784000804000F0A0417F41004000417FC067086708804001C067082C3C18782C3C804000";
   std::cout << "TEST 5: All 4 previous codes at once...\nCODE: " << testCode << "\n\n";
   RunInput(testCode);
}

void ADrawChallenge::RunTest6()
{
   std::string testCode = "F0A04000417F4000417FC04000400";
   std::cout << "TEST 6 : Cut off code...\nCODE: " << testCode << "\n\n";
   RunInput(testCode);
}
void ADrawChallenge::RunTest7()
{
   std::string testCode = "        F0A040004000417F417FC04000400090400047684F5057384000804001C05F204000400001400140400040007E405B2C4000804000";
   std::cout << "TEST 7: Space before code...\nCODE: " << testCode << "\n\n";
   RunInput(testCode);
}
void ADrawChallenge::RunTest8()
{
   std::string testCode = "F0A040004000417F417FC04000400090400047684F5057384000804001C05F 204000400001400140400040007E405B2C4000804000";
   std::cout << "TEST 8a: Space in code...\nCODE: " << testCode << "\n\n";
   RunInput(testCode);

   // remove space
   std::cout << "TEST 8b: Space removed...\nCODE: " << testCode << "\n\n";
   testCode.erase(remove_if(testCode.begin(), testCode.end(), isspace), testCode.end());
   RunInput(testCode);

}
void ADrawChallenge::RunTest9()
{
   std::string testCode = "3948ui9n34ijo5nu903489-v-143250 -14 8143 8-0m143um8-014358-m1438-m143-136y8-134-8adsfz,xmvn.";
   std::cout << "TEST 9: Just some garbage to see if anything breaks...\nCODE: " << testCode << "\n\n";
   RunInput(testCode);
}

void ADrawChallenge::RunTestDrawP()
{
   // mv (0,8000) (4000,0) (0,-4000) (-4000,0)
   // 4000 = 0
   // 7E40 = 8000
   // 5F20 = 4000
   // 2060 = -400

   std::string testCode = "F0"; // clear
   testCode += "A04000417F4000417F"; // green
   testCode += "804001";   // pen down
   testCode += "C040007E405F2040004000206020604000"; // mov to make my P
   testCode += "804000"; // pen up
   std::cout << "TEST P: My attempt at a green letter P...\nCODE: " << testCode << "\n\n";

   RunInput(testCode);
}