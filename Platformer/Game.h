#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "World.h"

class Game
{

private:
	int time_;
	int score_time_ = 1000000;

	enum GameState { kMenu, kPlayGame, kFinished };	//an enumeration type that determines the current state of the application

	GameState current_game_state_;						//a variable that stores the current state of the application

	bool resized_;									//a variable that stores the value_ of whether the window's value_ has changed since the last update
	sf::Font font_normal_, font_title_;					//variables storing fonts used in the game				
	sf::Text game_text_[3], menu_text_[2], game_title_, result_text_[3];	//variables storing text used in the game

	World mworld;								//a variable that stores the current level in the game and all the objects contained in it
	sf::RenderWindow m_window_;					//a variable that holds the application window
	float map_top_, map_bottom_, map_left_, map_right_; //variables storing the positions of the map edges
	static const sf::Time time_per_frame_;			//a variable that stores the time_ allocated to one frame
	bool key_pressed_[6] = { false };				//a variable that stores which button was pressed

	void menu();								//a function handling the game menu
	void HandleEvents();						//a function that supports the keyboard and closing and resizing the window
	void HandlePlayerInput(sf::Keyboard::Key key_code, bool is_pressed);	//a function that sets the values ??of the key_pressed_ table depending on the button pressed
	void UpdateView();							//a function that supports the in-game camera
	void RenderGame();							//a function that displays the game on the screen
	void UpdateGameText(int time);						//a function that updates the text displayed during the game
	void OpenLevel(std::string level_name);			//a function that opens a level contained in an .lvl file
	void run();									//a function that supports gameplay in a given level
	void GameResult(bool level_complete, int score);//a function that supports the results screen after the game
public:
	Game(sf::VideoMode resolution, std::string title);
	void start();	//function that starts the game
};

