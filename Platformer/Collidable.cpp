#include "Collidable.h"
#include <iostream>

Collidable::Collidable(sf::Vector2f position, sf::Vector2f size, object::Type obj_type): Entity( position,  size, obj_type)
{
}

collision::Type Collidable::IsColliding(Entity other, sf::Vector2f velocity)
{
	
	sf::FloatRect this_obj = obj_.getGlobalBounds();
	sf::FloatRect other_obj = other.GetBoundingRect();
	
	if (!this_obj.intersects(other_obj))
		return collision::kNoCollision;
	else
	{
		
		sf::Vector2f pos1 = obj_.getPosition();
		sf::Vector2f pos2 = other.GetPosition();
		sf::RectangleShape prev(obj_);
		prev.move(-velocity);
		sf::FloatRect prevRect = prev.getGlobalBounds();

		sf::RectangleShape top(other.GetRectangle()),bottom(other.GetRectangle()),left(other.GetRectangle()),right(other.GetRectangle());
		
		
		top.move(sf::Vector2f{ 0.0f,-top.getSize().y });
		bottom.move(sf::Vector2f{ 0.0f,bottom.getSize().y });
		left.move(sf::Vector2f{ -left.getSize().x,0.0f});
		right.move(sf::Vector2f{ left.getSize().x,0.0f });
		
	
		if (top.getGlobalBounds().intersects(prevRect))
			return collision::kTop;
		if (bottom.getGlobalBounds().intersects(prevRect))
			return collision::kBottom;
		if (left.getGlobalBounds().intersects(prevRect))
			return collision::kLeft;
		if (right.getGlobalBounds().intersects(prevRect))
			return collision::kRight;

		float delta_x = pos1.x - pos2.x;
		float delta_y = pos1.y - pos2.y;
		
	
		
		
		if (abs( (delta_x/other.GetSize().x) )> abs( (delta_y/other.GetSize().y)) )
		{
			if (delta_x < 0)
				return collision::kLeft;
			else
				return collision::kRight;

		}
		else
		{
			if (delta_y < 0)
				return collision::kTop;
			else
				return collision::kBottom;
		}
		
	}
}

void Collidable::SetVelocity(sf::Vector2f velocity)
{
	velocity_ = velocity;
}


sf::Vector2f Collidable::GetVelocity()
{
	return velocity_;
}

void Collidable::move(sf::Vector2f velocity)
{
	obj_.move(velocity);
}


