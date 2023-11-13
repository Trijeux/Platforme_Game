#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "AnimationHolder.h"
class Coin :
	public Entity
{
private:
	int value_;																		//a variable that stores the value_ of the coin
	AnimationHolder* animation_;
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;	//a function that overwrites the library draw function
public:
	Coin(sf::Vector2f position, sf::Vector2f size);
	Coin(sf::Vector2f position, sf::Vector2f size, int value);
	~Coin();

	void AddAnimation(sf::Texture& texture, sf::Time duration, unsigned int frameNumber, bool repeat);//a function that adds animations_ 
	void UpdateAnimation(sf::Time dt);		//a function that updates animations_
	int GetValue();							//a function that returns the value_ of the coin
};

