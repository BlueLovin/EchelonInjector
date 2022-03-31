#include "Game.h"

BOOL Game::isAlive()
{
	return Processes::GetProcId(L"splintercell.exe") != NULL;
}

std::string Game::GetMapName()
{
	std::string String;
	char _string[MAX_PATH];

	ReadProcessMemory(processHandle, (LPVOID)mapNameAddress, &_string, sizeof(_string), 0);
	String = std::string(_string);

	return String;
}

Game::Game()
{
	processHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);

	WinDrvDLLBaseAddress = Processes::GetModuleBaseAddress(processId, L"WinDrv.dll");
	soundDLLBaseAddress = Processes::GetModuleBaseAddress(processId, L"SNDDSound3DDLL_VBR.dll");
	DareAudioDLLBaseAddress = Processes::GetModuleBaseAddress(processId, L"DareAudio.dll");
	echelonDLLBaseAddress = Processes::GetModuleBaseAddress(processId, L"Echelon.dll");

	mapNameAddress = soundDLLBaseAddress + 0x6146c;
}