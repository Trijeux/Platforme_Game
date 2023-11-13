#include "Entity.h"



void Entity::draw(sf::RenderTarget & target, sf::RenderStates state) const
{
	target.draw(obj_, state);
}

void Entity::SetPosition(sf::Vector2f position)
{
	obj_.setPosition(position);
}

void Entity::SetXPosition(float x)
{
	obj_.setPosition(sf::Vector2f{ x,obj_.getPosition().y });
}

void Entity::SetYPosition(float y)
{
	obj_.setPosition(sf::Vector2f{ obj_.getPosition().x, y });
}

bool Entity::IsDestroyed()
{
	return destruction_flag_;
}

void Entity::destroy()
{
	destruction_flag_=true;
}

sf::Vector2f Entity::GetPosition()
{
	return obj_.getPosition();
}



sf::FloatRect Entity::GetBoundingRect()
{
	return obj_.getGlobalBounds();
}

sf::RectangleShape Entity::GetRectangle()
{
	return obj_;
}

float Entity::GetLeft()
{
	return obj_.getGlobalBounds().left;
}

float Entity::GetRight()
{
	return obj_.getGlobalBounds().left+obj_.getSize().x;
}

float Entity::GetTop()
{
	return obj_.getGlobalBounds().top;
}

float Entity::GetBottom()
{
	return obj_.getGlobalBounds().top+obj_.getSize().y;
}

sf::Vector2f Entity::GetSize()
{
	return obj_.getSize();
}

sf::Vector2f Entity::GetOrigin()
{
	return obj_.getOrigin();
}

void Entity::SetTexture(sf::Texture & texture, bool repeated)
{
	texture.setRepeated(repeated);

	obj_.setTexture(&texture);
	obj_.setTextureRect(sf::IntRect(0,0,obj_.getSize().x,obj_.getSize().y));


}

void Entity::SetColor(sf::Color color)
{
	obj_.setFillColor(color);
}

bool Entity::OnScreen(sf::View current_view)
{
	if (this->GetBottom() < current_view.getCenter().y - current_view.getSize().y / 2)
		return false;
	else if (this->GetTop() > current_view.getCenter().y + current_view.getSize().y / 2)
		return false;
	else if (this->GetRight() < current_view.getCenter().x - current_view.getSize().x / 2)
		return false;
	else if (this->GetLeft() > current_view.getCenter().x + current_view.getSize().x / 2)
		return false;
	else
		return true;
}

Entity::Entity(sf::Vector2f position, sf::Vector2f size, object::Type obj_type): destruction_flag_(false), obj_type_(obj_type)
{
	//obj_.setFillColor(sf::Color::Green);
	obj_.setSize(size);
	obj_.setOrigin(size / 2.f);
	obj_.setPosition(position);
		
}
object::Type Entity::GetType()
{
	return obj_type_;
}



