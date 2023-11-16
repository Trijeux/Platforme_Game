#include "Game.h"

#include <iostream>

const sf::Time Game::time_per_frame_ = sf::seconds(1.0f / 30.0f);


Game::Game(sf::VideoMode resolution, std::string title): resized_(true), current_game_state_(kMenu)
{
	m_window_.create(resolution, title);
	if (font_normal_.loadFromFile("Fonts/munro.ttf"))
	{
		game_text_[0].setFont(font_normal_);
		game_text_[1].setFont(font_normal_);
		game_text_[2].setFont(font_normal_);
		menu_text_[0].setFont(font_normal_);
		menu_text_[1].setFont(font_normal_);
		result_text_[1].setFont(font_normal_);
		result_text_[2].setFont(font_normal_);
	}
	if (font_title_.loadFromFile("Fonts/pab.otf"))
	{
		game_title_.setFont(font_title_);
		result_text_[0].setFont(font_title_);
	}
	game_text_[0].setFillColor(sf::Color::White);
	game_text_[1].setFillColor(sf::Color::White);
	game_text_[2].setFillColor(sf::Color::White);
	menu_text_[0].setFillColor(sf::Color::Yellow);
	menu_text_[1].setFillColor(sf::Color::White);
	game_title_.setFillColor(sf::Color::Green);
	result_text_[0].setFillColor(sf::Color::White);
	result_text_[1].setFillColor(sf::Color::Yellow);
}

void Game::start()
{
	this->menu();
}

void Game::OpenLevel(std::string level_name)
{
	try {
		mworld.LoadTextures();
	}
	catch (...)
	{
		throw;
	}

	int x_size, y_size;
	std::ifstream file("Levels/" + level_name + ".lvl");
	if (file.good())
	{
		file >> x_size;
		file >> y_size;
		char** map;
		map = new char*[x_size];
		for (int i = 0; i < x_size; i++)
			map[i] = new char[y_size];

		for (int i = 0; i < y_size; i++)
		{
			for (int j = 0; j < x_size; j++)
			{
				char temp;
				file >> temp;
				if (temp != '\n')
					map[j][i] = temp;
			}
		}

		float current_x = 12.5f, current_y = 12.5f;
		for (int y = 0; y < y_size; y++)
		{
			for (int x = 0; x < x_size; x++)
			{

				switch (map[x][y])
				{
				case 'w':	if (x < x_size - 1 && map[x + 1][y] == 'w')
							{
								x++;
								int temp = x + 1;
								float objSize = 2.f;
								current_x += 25.f;
								while (temp < x_size - 1 && map[temp][y] == 'w')
								{
									objSize++;
									temp++;
									x++;
									current_x += 25.f;
								}
								mworld.AddObject(new Entity(sf::Vector2f{ (current_x + 12.5f - 25.f / 2.f*objSize),current_y }, sf::Vector2f{ 25 * objSize,25 }, object::Type::kWall));
								break;
							}
							else if (y < y_size - 1 && map[x][y + 1] == 'w')
							{
								map[x][y] = '.';
								int temp = y + 1;
								float objSize = 1.f;
								while (temp < y_size   && map[x][temp] == 'w')
								{
									map[x][temp] = '.';
									objSize++;
									temp++;
								}
								mworld.AddObject(new Entity(sf::Vector2f{ current_x,(current_y + 25 * objSize / 2 - 12.5f) }, sf::Vector2f{ 25,25 * objSize }, object::Type::kWall));
								break;
							}
							else
								mworld.AddObject(new Entity(sf::Vector2f{ current_x,current_y }, sf::Vector2f{ 25,25 }, object::Type::kWall)); break;

				case 's':	if (x < x_size - 1 && map[x + 1][y] == 's')
							{
								x++;
								int temp = x + 1;
								float objSize = 2.f;
								current_x += 25.f;
								while (temp < x_size && map[temp][y] == 's')
								{
									objSize++;
									temp++;
									x++;
									current_x += 25.f;
								}
								mworld.AddObject(new Entity(sf::Vector2f{ (current_x + 12.5f - 25.f / 2.f*objSize),current_y }, sf::Vector2f{ 25 * objSize,25 }, object::Type::kSpikes));
								break;
							}
							else
								mworld.AddObject(new Entity(sf::Vector2f{ current_x,current_y }, sf::Vector2f{ 25,25 }, object::Type::kSpikes)); break;
				case 'c': mworld.AddObject(new Coin(sf::Vector2f{ current_x,current_y }, sf::Vector2f{ 20,20 })); break;
				case 'p': mworld.AddObject(new Player(sf::Vector2f{ current_x,current_y }, sf::Vector2f{ 18.f,60.f })); break;
				case 'r': mworld.AddObject(new SpawnPoint(sf::Vector2f{ current_x,current_y }, sf::Vector2f{ 10.f,22.f })); break;
				case 'b': mworld.AddObject(new Bat(sf::Vector2f{ current_x,current_y }, sf::Vector2f{ 1.f,10.f })); break;
				case 'e': mworld.AddObject(new Portal(sf::Vector2f{ current_x,current_y }, sf::Vector2f{ 10.f,22.f })); break;
				}
				current_x += 25.f;
			}
			current_y += 25.f;
			current_x = 12.5f;

		}	
		map_top_ = 0;
		map_bottom_ = 25.f * y_size;
		map_left_ = 0;
		map_right_ = 25.f * x_size;
	}
	else
		throw;
}

void Game::menu()
{
	enum MenuOption { kPlay, kExit };
	MenuOption current_option=kPlay;

	sf::Clock clock;
	sf::Time time_since_last_update = sf::Time::Zero;
	game_title_.setString("BATS & COINS");
	game_title_.setCharacterSize(69);
	game_title_.setOrigin(game_title_.getLocalBounds().left + game_title_.getLocalBounds().width / 2.f, game_title_.getLocalBounds().top + game_title_.getLocalBounds().height/2.f);
	game_title_.setPosition(m_window_.getView().getCenter() - sf::Vector2f{ 0,200 });
	
	menu_text_[0].setString("Play Game");
	menu_text_[0].setOrigin(menu_text_[0].getLocalBounds().left + menu_text_[0].getLocalBounds().width / 2.f, menu_text_[0].getLocalBounds().top + menu_text_[0].getLocalBounds().height / 2.f);
	menu_text_[0].setPosition(m_window_.getView().getCenter() - sf::Vector2f{ 0,50 });
	
	menu_text_[1].setString("Exit Game");
	menu_text_[1].setOrigin(menu_text_[1].getLocalBounds().left + menu_text_[1].getLocalBounds().width / 2.f, menu_text_[1].getLocalBounds().top + menu_text_[1].getLocalBounds().height / 2.f);
	menu_text_[1].setPosition(m_window_.getView().getCenter() - sf::Vector2f{ 0,20 });

	while (current_game_state_ == kMenu && m_window_.isOpen())
	{
		sf::Time elapsed_time = clock.restart();
		time_since_last_update += elapsed_time;

		while (time_since_last_update > time_per_frame_)
		{

			time_since_last_update -= time_per_frame_;
			HandleEvents();
		}
		if (key_pressed_[key::Type::kUp] && current_option>0)
		{
			menu_text_[current_option].setFillColor(sf::Color::White);
			current_option=MenuOption(current_option-1);
			menu_text_[current_option].setFillColor(sf::Color::Yellow);
		}
		if (key_pressed_[key::Type::kDown] && current_option < kExit)
		{
			menu_text_[current_option].setFillColor(sf::Color::White);
			current_option = MenuOption(current_option + 1);
			menu_text_[current_option].setFillColor(sf::Color::Yellow);
		}
		if (key_pressed_[key::Type::kEnter])
		{
			switch (current_option)
			{
			case MenuOption::kPlay: current_game_state_ = GameState::kPlayGame;
								   try
							   	   {
									   this->OpenLevel("level_02");
								   }
								   catch (...)
								   {
									   std::cout << "CRITICAL ERROR, CLOSING GAME!" << std::endl;
									   m_window_.close();
								   }
								   this->run();
								   break;
			case MenuOption::kExit: m_window_.close();
			}
		}
		m_window_.clear();
		m_window_.draw(game_title_);
		m_window_.draw(menu_text_[0]);
		m_window_.draw(menu_text_[1]);
		m_window_.display();
	}
}
void Game::HandleEvents()
{
	sf::Event event;
	while (m_window_.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed: HandlePlayerInput(event.key.code,true); break;
		case sf::Event::KeyReleased: HandlePlayerInput(event.key.code, false); break;
		case sf::Event::Closed: m_window_.close(); break;
		case sf::Event::Resized: resized_ = true; break;
		}
	}

}

void Game::HandlePlayerInput(sf::Keyboard::Key key_code, bool is_pressed)
{
	
	switch (key_code)
	{
	case sf::Keyboard::A: key_pressed_[key::kLeft] = is_pressed; return;
	case sf::Keyboard::D: key_pressed_[key::kRight] = is_pressed; return;
	case sf::Keyboard::W: key_pressed_[key::kUp] = is_pressed; return;
	case sf::Keyboard::S: key_pressed_[key::kDown] = is_pressed; return;
	case sf::Keyboard::Enter: key_pressed_[key::kEnter] = is_pressed; return;
	}
}

void Game::UpdateView()
{

	if (mworld.GetPlayerVelocity() != sf::Vector2f{ 0,0 }||resized_)
	{
		
		sf::View next_view;
		sf::Vector2f window_size = static_cast<sf::Vector2f>(m_window_.getSize());
		sf::Vector2f player_pos = mworld.GetPlayerPostition();
		sf::Vector2f prev_center = m_window_.getView().getCenter();
		sf::Vector2f view_center = player_pos;
		
		if (player_pos.x + window_size.x / 2 > map_right_ && map_right_ - map_left_ > window_size.x)
			view_center.x = map_right_ - window_size.x / 2;
		if (player_pos.x - window_size.x / 2 < map_left_ && map_right_ - map_left_ > window_size.x)
			view_center.x = map_left_ + window_size.x / 2;
		if (player_pos.y - window_size.y / 2 < map_top_ && map_bottom_ - map_top_ > window_size.y)
			view_center.y = map_top_ + window_size.y / 2;
		if (player_pos.y + window_size.y / 2 > map_bottom_ && map_bottom_ - map_top_ > window_size.y)
			view_center.y = map_bottom_ - window_size.y / 2;

		next_view.setCenter(view_center);
		next_view.setSize(window_size);
		mworld.SetCurrentView(next_view);
		m_window_.setView(next_view);
		resized_ = false;
	}
}



void Game::RenderGame()
{
	m_window_.clear(sf::Color(27,23,46));
	m_window_.draw(mworld);
	m_window_.draw(game_text_[0]);
	m_window_.draw(game_text_[1]);
	m_window_.draw(game_text_[2]);
	m_window_.display();
	
}

void Game::UpdateGameText(int time)
{
	game_text_[0].setString("Lives: " + std::to_string(mworld.PlayerHealth()));
	game_text_[1].setString("Coins: " + std::to_string(mworld.PlayerScore()/50)+"/"+std::to_string(mworld.GetPointTreshold()/50));
	game_text_[2].setString("Time: " + std::to_string(time));
	game_text_[0].setPosition(m_window_.getView().getCenter() - static_cast<sf::Vector2f>(m_window_.getSize()) / 2.f + sf::Vector2f{ 25.f,0.f });
	game_text_[1].setPosition(m_window_.getView().getCenter() - static_cast<sf::Vector2f>(m_window_.getSize()) / 2.f + sf::Vector2f{ 25.f,25.f });
	game_text_[2].setPosition(m_window_.getView().getCenter() - static_cast<sf::Vector2f>(m_window_.getSize()) / 2.f + sf::Vector2f{ 25.f,50.f });
	
}



void Game::run()
{
	sf::Clock clock;
	sf::Time time_since_last_update = sf::Time::Zero;
	
	while (current_game_state_==kPlayGame && m_window_.isOpen())
	{
		sf::Time elapsed_time = clock.restart();
		time_since_last_update += elapsed_time;

		while (time_since_last_update > time_per_frame_)
		{
		
			//auto start = std::chrono::steady_clock::now();
			time_since_last_update -= time_per_frame_;
			HandleEvents();
			if (!mworld.update(time_per_frame_, key_pressed_))
			{

				current_game_state_ = GameState::kFinished;
				GameResult(mworld.PlayerVictory(), mworld.PlayerScore());

			}
			//auto end = std::chrono::steady_clock::now();
			//auto diff = end - start;
			//if (std::chrono::duration <double, std::milli>(diff).count() > 3)
				//std::cout << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;
		}

		for (int i = 0; i < 1; i++) {
			time_++;
		}

		UpdateView();
		UpdateGameText(time_);
	
		RenderGame();
	}
}

void Game::GameResult(bool level_complete, int score)
{
	enum MenuOption { kPlay, kExit };
	sf::Clock clock;
	sf::Time time_since_last_update = sf::Time::Zero;

	if (level_complete)
	{
		result_text_[0].setString("CONGRATULATIONS! YOU WIN!");
		result_text_[0].setFillColor(sf::Color::Cyan);
		result_text_[0].setCharacterSize(40);
		result_text_[0].setOrigin(result_text_[0].getLocalBounds().left + result_text_[0].getLocalBounds().width / 2.f, result_text_[0].getLocalBounds().top + result_text_[0].getLocalBounds().height / 2.f);
		result_text_[0].setPosition(m_window_.getView().getCenter() - sf::Vector2f{ 0,200 });
	}
	else
	{
		result_text_[0].setString("GAME OVER! YOU LOSE!");
		result_text_[0].setFillColor(sf::Color::Red);
		result_text_[0].setCharacterSize(40);
		result_text_[0].setOrigin(result_text_[0].getLocalBounds().left + result_text_[0].getLocalBounds().width / 2.f, result_text_[0].getLocalBounds().top + result_text_[0].getLocalBounds().height / 2.f);
		result_text_[0].setPosition(m_window_.getView().getCenter() - sf::Vector2f{ 0,200 });
		score_time_ = 0;
		time_ = 0;
	}
	result_text_[1].setString("YOUR SCORE: " + std::to_string(score + 100 * mworld.PlayerHealth() + (score_time_ - time_)));
	result_text_[1].setOrigin(result_text_[1].getLocalBounds().left + result_text_[1].getLocalBounds().width / 2.f, result_text_[1].getLocalBounds().top + result_text_[1].getLocalBounds().height / 2.f);
	result_text_[1].setPosition(m_window_.getView().getCenter() - sf::Vector2f{ 0,50 });

	result_text_[2].setString("PRESS ENTER TO EXIT");
	result_text_[2].setOrigin(result_text_[2].getLocalBounds().left + result_text_[2].getLocalBounds().width / 2.f, result_text_[2].getLocalBounds().top + result_text_[2].getLocalBounds().height / 2.f);
	result_text_[2].setPosition(m_window_.getView().getCenter() - sf::Vector2f{ 0,20 });


	while (current_game_state_ == GameState::kFinished && m_window_.isOpen())
	{
		sf::Time elapsed_time = clock.restart();
		time_since_last_update += elapsed_time;

		while (time_since_last_update > time_per_frame_)
		{
			time_since_last_update -= time_per_frame_;
			HandleEvents();
		}
		if (key_pressed_[key::Type::kEnter])
		{
			m_window_.close();
		}
		m_window_.clear();
		m_window_.draw(result_text_[0]);
		m_window_.draw(result_text_[1]);
		m_window_.draw(result_text_[2]);
		m_window_.display();
	}
}



