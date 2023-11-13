#pragma once
#include "Entity.h"
#include "AnimationHolder.h"
class SpawnPoint :
	public Entity
{
private:
	AnimationHolder* animation_;
	bool is_enabled_;				//a variable storing the flag whether this is the spawn currently used by the player
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;//a function that overwrites the library draw function
public:
	SpawnPoint(sf::Vector2f position, sf::Vector2f size);
	~SpawnPoint();
	void SetEnabled(bool is_enabled);
	void AddAnimation(sf::Texture& texture, sf::Time duration, unsigned int frame_number, bool repeat);//a function that adds animations_
	void UpdateAnimation(sf::Time dt);//a function that updates animations_
};

