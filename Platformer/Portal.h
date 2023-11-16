#pragma once
#include "Entity.h"
#include "AnimationHolder.h"
class Portal :
	public Entity
{
private:
	bool open_;						//a variable that stores the flag whether the portal is open_ or not
	//AnimationHolder* animations_[2]; //a variable that stores all portal animations_
	std::vector<AnimationHolder> animations_;
	animation::Type current_animation_;	//a variable that stores the animation_ update type
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;//a function that overwrites the library draw function
public:
	Portal(sf::Vector2f position, sf::Vector2f size);
	void AddAnimation(animation::Type anim_type, sf::Texture& texture, sf::Time duration, unsigned int frame_number, bool repeat);//a function that adds animations_ 
	void UpdateAnimation(sf::Time dt);//a function that updates animations_
	bool IsOpen();	   //the function returns the true flag when the portal is open_
	void OpenPortal(); //a function that opens a given portal
	~Portal();

};

