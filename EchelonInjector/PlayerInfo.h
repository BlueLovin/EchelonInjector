#pragma once
#include "EchelonInjector.h"
#include "Game.h"
class PlayerInfo
{
private:
	uintptr_t xVelocityBaseAddress, yVelocityBaseAddress;
	std::vector<unsigned int> xVelocityOffsets, yVelocityOffsets;
	void InitializePointers();
	void RelocatePointers();
	Game game;
	uintptr_t xVelocityAddress, yVelocityAddress = 0x00;

public:
	float xVelocity, yVelocity = 1.0f;
	void UpdateVelocity();
	float GetSpeed();
	PlayerInfo(Game _game);
};
