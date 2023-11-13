#pragma once
#include "Entity.h"
namespace collision
{
	enum Type { kNoCollision, kTop, kBottom, kLeft, kRight };	//an enumeration type that specifies the type of collision,,
	//for example, if an object collided with the top of another object
	//then the collision type will be kTop
}

class Collidable :
	public Entity
{
protected:
	sf::Vector2f velocity_;	//a variable that stores the current velocity_ vector of the object
public:
	Collidable(sf::Vector2f position, sf::Vector2f size, object::Type obj_type);
	collision::Type IsColliding(Entity other, sf::Vector2f velocity);				//a function that returns the type of collision between this object and another,
																					//if no collision occurs, it will return kNoCollision
	void SetVelocity(sf::Vector2f velocity);
	sf::Vector2f GetVelocity();
	void move(sf::Vector2f velocity);									//a function that moves an object by a given vector
	virtual void ResolveCollision(Entity& other) = 0;						//a virtual function that resolves collisions

};

