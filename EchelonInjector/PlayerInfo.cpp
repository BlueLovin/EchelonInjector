#include "PlayerInfo.h"

void PlayerInfo::UpdateVelocity()
{
	RelocatePointers(); //this function call is expensive and evil
	ReadProcessMemory(game.processHandle, (BYTE *)xVelocityAddress, &xVelocity, sizeof(xVelocity), nullptr);
	ReadProcessMemory(game.processHandle, (BYTE *)yVelocityAddress, &yVelocity, sizeof(yVelocity), nullptr);
}

float PlayerInfo::GetSpeed()
{
	// thank u nambona! :-) | pythagorean theorem
	return sqrt((xVelocity * xVelocity) + (yVelocity * yVelocity));
}

void PlayerInfo::InitializePointers()
{
	// resolving pointer offsets
	xVelocityBaseAddress = game.DareAudioDLLBaseAddress + 0x0000cbdc;
	xVelocityOffsets = { 0xec };
	xVelocityAddress = Processes::FindDMAAddy(game.processHandle, xVelocityBaseAddress, xVelocityOffsets);

	yVelocityBaseAddress = game.DareAudioDLLBaseAddress + 0x0000cbdc;
	yVelocityOffsets = { 0xf0 };
	yVelocityAddress = Processes::FindDMAAddy(game.processHandle, yVelocityBaseAddress, yVelocityOffsets);
}

PlayerInfo::PlayerInfo(Game _game)
{
	game = _game;

	InitializePointers();
}

void PlayerInfo::RelocatePointers()
{
	yVelocityAddress = Processes::FindDMAAddy(game.processHandle, yVelocityBaseAddress, yVelocityOffsets);
	xVelocityAddress = Processes::FindDMAAddy(game.processHandle, xVelocityBaseAddress, xVelocityOffsets);
}