#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include "Player.h"
#include "Collidable.h"
#include "Entity.h"
#include "TextureHolder.h"
#include "Bat.h"
#include "Coin.h"
#include "Portal.h"

class World :
	public sf::Drawable
{
private:
	const static int point_treshold_;		//a variable storing the amount of coins needed to open_ the portal
	sf::View current_view_;					//a variable storing the current camera view
	TextureHolder textures_;				//a variable that stores all textures_ in the game
	sf::Sprite background;				//a variable storing the game background
	Player* m_player_;					//a variable storing a pointer to the player
	std::vector<Entity*> m_blocks_;		//a variable storing a vector of pointers to all static objects
	std::vector<SpawnPoint*> m_spawn_points_; //a variable storing a vector of pointers to all spawns 
	std::vector<Bat*> m_enemies_;			//a variable storing a vector of pointers to all opponents
	std::vector<Coin*> m_coins_;			//a variable storing a vector of indicators for all coins
	Portal* m_portal_;					//a variable storing a pointer to the portal



	void MultithreadEnemyCollisions(std::vector<Bat*> enemies, int firstIndex, int n); //a function that resolves adversarial collisions using multithreading
	void HandleBetterEnemyCollisions();	//a function that calls a multi-threaded function for resolving opponent collisions
	void UpdateEnemies(sf::Time dt);	//a function that updates the movement of all opponents
	void HandlePlayerCollisions();		//a function that resolves player collisions
	void UpdateAnimations(sf::Time dt);//a function that updates all animations_ of objects in the game
	void CleanCoins();					//a function that removes collected coins
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;//a function that overwrites the library draw function
	void UpdateBackground();			//a function that creates the background

public:

	World();
	~World();

	bool update(sf::Time dt, bool key_pressed[5]);
	sf::Vector2f GetPlayerPostition();
	void LoadTextures();				//a function that loads all the necessary textures_
	void AddObject(Player* new_player);	//a feature that adds to the player's world
	void AddObject(Entity* new_block);	//a function that adds a static object to the world
	void AddObject(SpawnPoint* new_spawn_point); //a function that adds the player's spawn to the world
	void AddObject(Bat* new_enemy);		//a function that adds an enemy to the world
	void AddObject(Coin* new_coin);		//a function that adds coins to the world
	void AddObject(Portal* new_portal);	//a function that adds a portal to the world
	int PlayerHealth();					//a function that takes the player's life and returns it
	int PlayerScore();					//a function that takes the player's points and returns them
	void SetCurrentView(sf::View current_view);
	int GetPointTreshold();
	bool PlayerVictory();				//a function that takes the player's victory flag and returns it
	sf::Vector2f GetPlayerVelocity();	//a function that takes the player's velocity_ vector and returns it
};


