/*
Copyright (c) 2019 IkarusDeveloper

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#ifdef _WIN64
#define PLATFORM_NAME_TO_LINK "x64"
#else
#define PLATFORM_NAME_TO_LINK "Win32"
#endif

#ifdef _DEBUG
#define CONFIGURATION_TO_LINK "Debug"
#else
#define CONFIGURATION_TO_LINK "Release"
#endif

#ifdef _DLL
#define GENERATION_NAME_TO_LINK " MD"
#else
#define GENERATION_NAME_TO_LINK
#endif



#pragma comment(lib, "ikarusgl-" CONFIGURATION_TO_LINK GENERATION_NAME_TO_LINK "-" PLATFORM_NAME_TO_LINK ".lib")
#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "Dwmapi.lib")