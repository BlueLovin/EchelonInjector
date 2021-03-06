#pragma once
#include "Processes.h"
#include <string>

class Game {
public:
	DWORD processId = Processes::GetProcId(L"splintercell.exe");
	BOOL isAlive();
	uintptr_t WinDrvDLLBaseAddress;
	uintptr_t DareAudioDLLBaseAddress;
	HANDLE processHandle;
	std::string GetMapName();
	Game();
private:
	uintptr_t soundDLLBaseAddress;
	uintptr_t mapNameAddress;
};