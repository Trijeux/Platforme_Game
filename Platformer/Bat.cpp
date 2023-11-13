#include "Bat.h"
#include <iostream>
const float Bat::movement_speed_ = 120.f;

void Bat::draw(sf::RenderTarget & target, sf::RenderStates state) const
{
	if (animation_ != nullptr)
	{
		target.draw(*animation_, state);
		//target.draw(obj_, state);
	}
	else
		target.draw(obj_, state);
}

Bat::Bat(sf::Vector2f position, sf::Vector2f size): Collidable(position,size,object::Type::kEnemy)
{
	velocity_.y = 0;
	if(rand()%2)
		velocity_.x = movement_speed_;
	else
		velocity_.x = -movement_speed_;
	this->SetColor(sf::Color::Yellow);
}

void Bat::AddAnimation(sf::Texture & texture, sf::Time duration, unsigned int frame_number, bool repeat)
{
	if (animation_ != nullptr)
		delete(animation_);
	animation_ = new AnimationHolder(texture, duration, frame_number, repeat);
	animation_->SetPosition(this->obj_.getPosition());
	animation_->SetColor(sf::Color::Cyan);
	if (velocity_.x < 0)
		animation_->swap();
}

void Bat::UpdateAnimation(sf::Time dt)
{
	if(animation_->GetDirection()==animation::Direction::kRight)
		animation_->SetPosition(this->GetPosition() - sf::Vector2f{5.f,-2.f});
	else
		animation_->SetPosition(this->GetPosition() + sf::Vector2f{ 5.f,2.f });
	if(animation_!=nullptr)
		animation_->update(dt);
}

void Bat::ResolveCollision(Entity & other)
{
	collision::Type c_type = IsColliding(other, velocity_);
	if (c_type == collision::kNoCollision)
	{
		return;
	}

	switch (other.GetType())
	{
	case object::kWall:
		switch (c_type)
		{
		case collision::kLeft: SetXPosition(other.GetLeft() - GetSize().x / 2.0f); velocity_ = -velocity_; animation_->swap();  //std::cout << "B: kLeft" << std::endl;
			return;
		case collision::kRight: SetXPosition(other.GetRight() + GetSize().x / 2.0f); velocity_ = -velocity_; animation_->swap(); //std::cout << "B: kRight" << std::endl;
			return;
		case collision::kTop: SetYPosition(other.GetTop() - GetSize().y / 2.0f); //::cout << "B: kTop" << std::endl;
			return;
		case collision::kBottom: SetYPosition(other.GetBottom() + GetSize().y / 2.0f); //std::cout << "B: kBottom" << std::endl;
			return;
		}
		break;
	case object::kPlayer:
		switch (c_type)
		{
		case collision::kLeft:
		case collision::kRight:
		case collision::kTop:
		case collision::kBottom:
			dynamic_cast<Player&>(other).hurt(); //std::cout << "B: Player" << std::endl;
			return;
		}
		break;
	}
}

void Bat::update(sf::Time dt)
{
	obj_.move(velocity_*dt.asSeconds());
}


Bat::~Bat()
{
}
