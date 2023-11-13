#pragma once
#include "Collidable.h"
#include "AnimationHolder.h"
#include "Player.h"
class Bat :
	public Collidable
{
private:
	const static float movement_speed_;												//a variable that stores the enemy's movement speed
	AnimationHolder* animation_;														//a variable that stores the opponent's animations_
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;	//a function that overwrites the library draw function
public:
	Bat(sf::Vector2f position, sf::Vector2f size);
	void AddAnimation(sf::Texture& texture, sf::Time duration, unsigned int frame_number, bool repeat);	//a function that adds animations_ 
	void UpdateAnimation(sf::Time dt);										//a function that updates animations_
	void ResolveCollision(Entity& other) override;							//a function that resolves collisions between an object and other objects
	void update(sf::Time dt);												//a function that updates the opponent's movement
	~Bat();
};

