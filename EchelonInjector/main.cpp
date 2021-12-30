#include "Shlwapi.h"
#include <string>
#include <sstream>
#include <thread>
#include <iostream>
#include "../EchelonInjector/Game.h"
#include "../EchelonInjector/PlayerInfo.h"
#include "Drawing.cpp"

std::wstring FloatToWString(float f) {
	std::ostringstream ss;
	ss << f;
	std::string s(ss.str());

	std::wstring wstr(s.begin(), s.end());
	return wstr;
}

int RenderTextOverGame()
{
	Game game = Game();

	if (game.processId == NULL) {
		MessageBox(NULL, L"COULD NOT FIND SPLINTER CELL PROCESS!", NULL, NULL);
		return -1;
	}

	PlayerInfo player = PlayerInfo(game);

	HFONT font = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Sans Serif");

	while (true)
	{
		if (!game.isAlive()) {
			break;
		}
		// update player speed
		player.UpdateVelocity();
		float speed = player.GetSpeed();
		std::wstring speedStr = FloatToWString(speed);

		//show speed string
		Drawing::Show(speedStr, font, RGB(255, 0, 0));

		Drawing::Wait_ms(75);
	}

	return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	Drawing::Init(hInstance);

	return RenderTextOverGame();
}