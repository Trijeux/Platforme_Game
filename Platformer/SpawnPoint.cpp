#include "SpawnPoint.h"





void SpawnPoint::draw(sf::RenderTarget & target, sf::RenderStates state) const
{
	if (animation_ != nullptr)
	{
		target.draw(*animation_, state);
		//target.Draw(obj_, state);
	}
	else
		target.draw(obj_, state);
}

SpawnPoint::SpawnPoint(sf::Vector2f position, sf::Vector2f size): Entity(position, size, object::Type::kSpawnPoint), is_enabled_(false)
{
}

SpawnPoint::~SpawnPoint()
{
	delete animation_;
}

void SpawnPoint::SetEnabled(bool is_enabled)
{
	is_enabled_ = is_enabled;
}

void SpawnPoint::AddAnimation(sf::Texture & texture, sf::Time duration, unsigned int frame_number, bool repeat)
{
	if (animation_ != nullptr)
		delete(animation_);
	animation_ = new AnimationHolder(texture, duration, frame_number, repeat);
	animation_->SetPosition(this->obj_.getPosition());
}

void SpawnPoint::UpdateAnimation(sf::Time dt)
{
	if (is_enabled_)
		animation_->SetColor(sf::Color::Green);
	else
		animation_->SetColor(sf::Color::White);
	animation_->update(dt);
}
