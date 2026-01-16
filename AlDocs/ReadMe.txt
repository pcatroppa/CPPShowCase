Prerequisites:
Visual Studio 2015 or newer.

Build Instructions:
1) Open file "DrawChallenge.sln" in Visual Studio 2015 (may need to convert if using newer VS).
2) Make sure "DrawChallenge" is set as "Startup Project".
3) Make sure you are set to "Release x86" or "Debug x86" 
(although it should work in x64 the "Tests" project may fail).
4) Build (F7) - should be 0 errors and 0 warnings.
4) Run (F5)

Test Instructions:
1) In the "DrawChallenge.sln" look for project "Tests".
2) Right click and build (or rebuild) the project.
3) Choose "Test -> Run -> All Tests" from the menu in Visual Studio.
4) All 10 tests should pass.
(As mentioned these tests only work for x86 debug or release configurations)

There are also built in tests for the end user available in application.
1) Run (or debug) the DrawChallenge.exe
2) Choose t to test using the provided examples
3) Choose x for extended tests.  
(The last test should draw a green P)

Release Instructions:
This project is for testing purposes and has no release instructions.
