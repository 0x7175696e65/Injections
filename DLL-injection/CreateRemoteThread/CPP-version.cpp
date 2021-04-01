#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

// we pass in the process name
DWORD GetProcId(const char* procName) {
// we create a processId and initialize it with 0 
// this will be returned with the real value in the end 
	DWORD procId = 0;
// Next, we create a handle to take a snapshot of all the running processes
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hsnap != INVALID_HANDLE_VALUE) {

		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hsnap, &procEntry)) {

			do {

				if (!_stricmp(procEntry.szExeFile, procName)) {
					
					procId = procEntry.th32ProcessID;
					break;
				}

			} while (Process32Next(hsnap, &procEntry));
		}
	}
	CloseHandle(hsnap);
	return procId;
}

int main() {
	const char * dllPath = "C:\\Users\\admin\\Desktop\\New folder\\hello-world-dll-master\\Release\\x64\\hello-world.dll";
	const char * procName = "notepad.exe";
	DWORD procId = 0;


	while (!procId) {
	
		procId = GetProcId(procName);
		Sleep(30);
	}

	std::cout << procId;

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);
	printf("A\n");

	if (hProc && hProc != INVALID_HANDLE_VALUE) {
	
		printf("B\n");
		void * loc = VirtualAllocEx(hProc,  0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		printf("C\n");

		if (loc) {
			printf("D\n");
			WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);
			printf("E\n");
		}

		HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
		printf("F\n");

		if (hThread) {
			printf("G\n");
//			CloseHandle(hThread);
	//		printf("H\n");
		}

	}

	if (hProc) {
	
		CloseHandle(hProc);
	}

	return 0;
}
