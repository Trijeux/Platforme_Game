#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Windows.h>
#include "Game.h"

int main()
{

	HWND h_wnd = GetConsoleWindow();
	ShowWindow(h_wnd, SW_HIDE);
	srand(time(NULL));
	Game m_game(sf::VideoMode{ 800,600 }, "Platformer");
	m_game.start();
	
}
