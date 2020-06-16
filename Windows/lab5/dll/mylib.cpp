
#include "stdafx.h"
#include <iostream>
#include "mylib.h"

BOOL ReadTestFile(const char* filePath, const char* message)
{
	std::cout << "Read file - start" << std::endl;

	OVERLAPPED overlappedStructure;
	overlappedStructure.Offset = NULL;
	overlappedStructure.OffsetHigh = NULL;
	overlappedStructure.hEvent = NULL;
	HANDLE readFile = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	ReadFile(readFile, (LPVOID)message, 80, NULL, &overlappedStructure);
	WaitForSingleObject(readFile, INFINITE); 
	CloseHandle(readFile);	
	std::cout << "Read file - finish" << std::endl;
	return TRUE;
}

BOOL WriteTestFile(const char* filePath, const char* message, unsigned int size, unsigned int offset)
{
	std::cout << "Write file - start" << std::endl;

	OVERLAPPED overlappedStructure;
	overlappedStructure.Offset = offset;
	overlappedStructure.OffsetHigh = NULL;
	overlappedStructure.hEvent = NULL;
	HANDLE writeFile = CreateFileA(filePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	WriteFile(writeFile, message, size, NULL, &overlappedStructure);
	WaitForSingleObject(writeFile,  INFINITE)
	CloseHandle(writeFile);	
	std::cout << "Write file - finish" << std::endl;
	return TRUE;
}



