// Copyright (c) 2018 Art & Logic, Inc. All Rights Reserved.
#pragma once

#ifndef h_DrawChallenge
#define h_DrawChallenge

#include <string>

/**
* class ADrawChallenge
* Handles the main user interface
* and entry point for the program.
* 
*/

class ADrawChallenge
{
public:
   ADrawChallenge();
   // User Interface Helpers
   void LoopUserInterface();  
   void CleanInput(std::string& commandInput);
   void RunInput(const std::string& commandInput);

private:
   // toggle for cleaning white spaces and non alphanumeric
   bool bCleanInput;

   // Tests
   void RunTestDrawP(); 
   void RunTest1();
   void RunTest2();
   void RunTest3();
   void RunTest4();
   void RunTest5();
   void RunTest6();
   void RunTest7();
   void RunTest8();
   void RunTest9();
   void RunBasicTests();
   void RunAllTests();

};

#endif