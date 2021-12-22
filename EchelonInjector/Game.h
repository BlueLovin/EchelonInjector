#pragma once
#include "Processes.h"

class Game {
public:
	DWORD processId = Processes::GetProcId(L"splintercell.exe");
	uintptr_t echelonDLLBaseAddress = Processes::GetModuleBaseAddress(processId, L"Echelon.dll");
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);
};