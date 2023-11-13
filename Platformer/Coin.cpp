#include "Coin.h"






void Coin::draw(sf::RenderTarget & target, sf::RenderStates state) const
{
	if (animation_ != nullptr)
	{
		target.draw(*animation_, state);
		//target.Draw(obj_, state);
	}
	else
		target.draw(obj_, state);
}

Coin::Coin(sf::Vector2f position, sf::Vector2f size): Entity(position,size,object::Type::kCoin), value_(50)
{
}

Coin::Coin(sf::Vector2f position, sf::Vector2f size, int value): Entity(position, size, object::Type::kCoin), value_(value)
{
}

Coin::~Coin()
{
	delete animation_;
}

void Coin::AddAnimation(sf::Texture & texture, sf::Time duration, unsigned int frameNumber, bool repeat)
{
	if (animation_ != nullptr)
		delete(animation_);
	animation_ = new AnimationHolder(texture, duration, frameNumber, repeat);
	animation_->SetPosition(this->obj_.getPosition());
	animation_->SetColor(sf::Color::Yellow);
	
}

void Coin::UpdateAnimation(sf::Time dt)
{
	animation_->update(dt);
}

int Coin::GetValue()
{
	return value_;
}
