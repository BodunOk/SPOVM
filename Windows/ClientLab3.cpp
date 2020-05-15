#include <iostream>
#include <Windows.h>
#include <string>

int main() {
	HANDLE buffer = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, L"buffer");
	HANDLE canPrint = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, L"canPrint");
	HANDLE pipe = CreateFileA("\\\\.\\pipe\\ipctest", GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pipe == INVALID_HANDLE_VALUE) {
		std::cout << "Error creating file" << std::endl;
		exit(0);
	}
	DWORD read = 0;
	char buff[80];
	while (true) {
		WaitForSingleObject(canPrint, INFINITE);
		memset(buff, 0, 80);
		ReadFile(pipe, buff, 79, &read, NULL);
		std::string fromServer(buff);
		if (fromServer == "stop") {
			CloseHandle(pipe);
			exit(0);
		}
		std::cout << "Client : " << fromServer << std::endl;
		ReleaseSemaphore(buffer, 1, NULL);
	}
	CloseHandle(buffer);
	CloseHandle(canPrint);
	CloseHandle(pipe);
}