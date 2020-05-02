#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <map>

const wchar_t* toWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new WCHAR[cSize];
	mbstowcs(wc, c, cSize);
	return wc;
}

LPWSTR toWS(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new WCHAR[cSize];
	mbstowcs(wc, c, cSize);
	return wc;
}

HANDLE newProc(int procNumber, const std::string& parentPath)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	const std::wstring path = toWC(parentPath.c_str());
	char args[3];
	sprintf(args + 1, "%d", procNumber);
	args[0] = ' ';

	if (!CreateProcess(path.c_str(), toWS(args), NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi))
	{
		std::cout << "Failed creating process" << std::endl;
		exit(-1);
	}
	return pi.hProcess;
}

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		HANDLE writeEvent = OpenEvent(EVENT_ALL_ACCESS, false, toWS((std::string("WriteEvent_") + argv[argc - 1]).c_str()));

		while (true)
		{
			std::string pid = "process";
			WaitForSingleObject(writeEvent, INFINITE);

			for (size_t i = 0; i < pid.size(); i++)
			{
				printf_s("%c", pid[i]);
				Sleep(50);
			}
			for (size_t i = 0; i < strlen(argv[argc - 1]); i++)
			{
				printf_s("%c", argv[argc - 1][i]);
			}
			printf_s("\n");
			ResetEvent(writeEvent);
			while (WaitForSingleObject(writeEvent, 0) == WAIT_OBJECT_0);
		}
	}

	std::vector<std::pair<HANDLE, HANDLE>> children;
	children.reserve(5);
	unsigned currentWriting = 0;
	bool exitFlag = false;

	while (true)
	{
		while (_kbhit())
		{
			switch (_getch())
			{
			case 'q':
				while (!children.empty())
				{
					std::pair<HANDLE, HANDLE> child = children.back();
					TerminateProcess(child.first, EXIT_SUCCESS);
					CloseHandle(child.second);
					CloseHandle(child.first);
					children.pop_back();
				}
				exitFlag = true;
				break;
			case '+':
				if (children.size() < 5)
				{
					HANDLE WriteEvent = CreateEvent(nullptr, TRUE, FALSE,
						toWS((std::string("WriteEvent_") + std::to_string(children.size() + 1)).c_str()));
					HANDLE process = newProc(children.size() + 1, argv[0]);
					children.emplace_back(process, WriteEvent);

				}
				break;

			case '-':
				if (!children.empty()) {
					const std::pair<HANDLE, HANDLE> Child = children.back();
					TerminateProcess(Child.first, EXIT_SUCCESS);
					CloseHandle(Child.second);
					CloseHandle(Child.first);
					children.pop_back();
					if (children.empty())
						std::cout << "\nNo more processes." << std::endl;
				}
				else
					std::cout << "\nNo more processes running." << std::endl;
				break;

			default:;
			}
		}
		if (exitFlag) {
			break;
		}
		if (!children.empty()) {
			if (++currentWriting >= children.size())
				currentWriting = 0;
			SetEvent(children[currentWriting].second);
			while (WaitForSingleObject(children[currentWriting].second, 0) == WAIT_OBJECT_0);
		}
	}
}