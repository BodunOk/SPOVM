#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <Windows.h>
#include <list>
#include <tchar.h>

using namespace std;

void printProcessSignature(string rocNumber);

const int DELAY = 250;

int main(int argc, char* argv[]) {
	HANDLE eventHdl = OpenEvent(EVENT_ALL_ACCESS, false, TEXT("writeEvent"));

	string information = argv[0];

	while (true) {
		WaitForSingleObject(eventHdl, INFINITE);
		printProcessSignature(information);
		SetEvent(eventHdl);
	}
}


void printProcessSignature(string procNumber) {

	for (int i = 0; i < procNumber.length(); i++) {
		cout << procNumber[i];
	}

	string message = " Process";
	for (int i = 0; i < message.length(); i++) {
		cout << message[i];
		//Sleep(DELAY);
	}
	cout << endl;

	Sleep(DELAY);
}