#include "EchelonInjector.h"
#include "Game.h"

void clear()
{
	// CSI[2J clears screen, CSI[H moves the cursor to top-left corner
	std::cout << "\x1B[2J\x1B[H";
}

int main()
{
	Game game = Game();
	PlayerInfo player = PlayerInfo(game);

	if (game.processId == NULL) {
		std::cout << "COULD NOT FIND SPLINTER CELL PROCESS!\n";
		getchar();
		return -1;
	}

	while (true)
	{
		player.UpdateVelocity();
		clear();
		COORD coord;
		coord.X = 0;
		coord.Y = 0;
		//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		std::cout << "xVelocity = " << std::dec << player.xVelocity << std::endl;
		std::cout << "yVelocity = " << std::dec << player.yVelocity << std::endl;
		std::cout << "speed = " << std::dec << player.GetSpeed() << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	////Write to it
	//int newAmmo = 1337;
	//WriteProcessMemory(hProcess, (BYTE *)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

	////Read out again
	//ReadProcessMemory(hProcess, (BYTE *)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);

	//std::cout << "New ammo = " << std::dec << ammoValue << std::endl;

	getchar();
	return 0;
}