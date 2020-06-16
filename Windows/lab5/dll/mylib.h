#pragma once

#ifdef MYLIB_EXPORTS
#define MYLIB_API __declspec(dllexport)
#else
#define MYLIB_API __declspec(dllimport)
#endif

extern "C" __declspec(dllexport) BOOL ReadTestFile(const char* filePath, const char* message);
extern "C" __declspec(dllexport) BOOL WriteTestFile(const char* filePath, const char* message, unsigned int size, unsigned int offset);
