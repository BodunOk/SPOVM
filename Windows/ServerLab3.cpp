#include <iostream>
#include <Windows.h>
#include <string>

int main() {
	CreateSemaphore(NULL, 1, 1, L"buffer");
	CreateSemaphore(NULL, 0, 1, L"canPrint");
	HANDLE buffer = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, L"buffer");
	HANDLE canPrint = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, L"canPrint");
	HANDLE pipe = CreateNamedPipeA("\\\\.\\pipe\\ipctest", PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE, 2, 80, 80,
		20000000, NULL);
	DWORD written = 0;
	if (buffer == 0 || canPrint == 0) {
		std::cout << "Error creating semaphore" << std::endl;
	}
	std::string toClient;
	STARTUPINFO si;
	si.cb = sizeof(si);
	ZeroMemory(&si, sizeof(si));
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	CreateProcess(
		L"C:\Users\abutg\source\repos\lab3SpovmClient\Debug\lab3SpovmClient.exe",
		NULL,
		NULL,
		NULL,
		FALSE,
		NULL,
		NULL,
		NULL,
		&si,
		&pi);
	do {
		WaitForSingleObject(buffer, INFINITE);
		std::cout << "Server : ";
		std::cin >> toClient;
		WriteFile(pipe, toClient.c_str(), toClient.length(), &written, NULL);
		ReleaseSemaphore(canPrint, 1, NULL);

	} while (toClient != "stop");
	CloseHandle(pipe);
	CloseHandle(buffer);
	CloseHandle(canPrint);
	return 0;
}