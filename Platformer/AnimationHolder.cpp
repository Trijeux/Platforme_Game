#include "AnimationHolder.h"
#include <iostream>


void AnimationHolder::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_sprite_, states);
}

AnimationHolder::AnimationHolder(sf::Texture&texture,sf::Time duration, unsigned int frame_number, bool repeat):
	m_sprite_(texture),
	anim_duration_(duration),
	repeat_(repeat),
	finished_(false),
	frame_number_(frame_number),
	current_frame_(0),
	elapsed_time_(sf::Time::Zero), 
	anim_direction_(animation::Direction::kRight)
{
	sf::FloatRect spriteBounds = m_sprite_.getGlobalBounds();
	m_sprite_.setOrigin(spriteBounds.left + spriteBounds.height / 2.f, spriteBounds.top + spriteBounds.height / 2.f);
	m_sprite_.setScale(2, 2);
	frame_size_ = sf::Vector2u(texture.getSize().x / frame_number_, texture.getSize().y);
}


void AnimationHolder::SetColor(sf::Color color)
{
	m_sprite_.setColor(color);
}

void AnimationHolder::SetPosition(sf::Vector2f pos)
{
	m_sprite_.setPosition(pos);
}

void AnimationHolder::update(sf::Time dt)
{
	sf::Time frame_duration = anim_duration_ / static_cast<float>(frame_number_);
	elapsed_time_ += dt;

	sf::Vector2u textureBounds(m_sprite_.getTexture()->getSize());
	sf::IntRect textureRect=m_sprite_.getTextureRect();

	if (current_frame_ == 0)
		textureRect = sf::IntRect(0, 0, frame_size_.x, frame_size_.y);

	while (elapsed_time_ >= frame_duration && (current_frame_ <= frame_number_ || repeat_))
	{
		
		elapsed_time_ -= frame_duration;
		++current_frame_;
			if (current_frame_ >= frame_number_)
			{
				if (repeat_)
				{
					current_frame_ = 0;
					textureRect = sf::IntRect(0, 0, frame_size_.x, frame_size_.y);
				}
					
				else
				{	
					current_frame_--;
					finished_=true;
				}
				
			}
			else
				textureRect.left += textureRect.width;
		
	}
		
	m_sprite_.setTextureRect(textureRect);
}

void AnimationHolder::swap()
{
	m_sprite_.scale(sf::Vector2f{ -1,1 });
	if (anim_direction_ == animation::Direction::kLeft)
		anim_direction_ = animation::Direction::kRight;
	else
		anim_direction_ = animation::Direction::kLeft;
}

void AnimationHolder::restart()
{
	current_frame_ = 0;
	finished_ = false;
}

bool AnimationHolder::IsFinished()
{
	return finished_;
}

animation::Direction AnimationHolder::GetDirection()
{
	return anim_direction_;
}

