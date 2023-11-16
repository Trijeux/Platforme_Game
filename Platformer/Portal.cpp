#include "Portal.h"



void Portal::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	if (current_animation_ < animations_.size())
	{
		target.draw(animations_[current_animation_], state);
		//target.Draw(obj_, state);
	}
	else
		target.draw(obj_, state);
}

void Portal::AddAnimation(animation::Type anim_type, sf::Texture& texture, sf::Time duration, unsigned int frame_number, bool repeat)
{
	if (anim_type <= animation::Type::kMove)
	{
		/*if (animations_[anim_type] != nullptr)
			delete(animations_[anim_type]);*/
		/*animations_[anim_type] = new AnimationHolder(texture, duration, frame_number, repeat);*/
		animations_.emplace_back(texture, duration, frame_number, repeat);
		animations_[anim_type].SetPosition(this->obj_.getPosition());
		animations_[anim_type].swap();
		animations_[anim_type].SetColor(sf::Color::Cyan);
	}
}

void Portal::UpdateAnimation(sf::Time dt)
{
	if (animations_[animation::Type::kMove].IsFinished())
		current_animation_ = animation::Type::kIdle;
	animations_[current_animation_].update(dt);
}

bool Portal::IsOpen()
{
	return open_;
}

void Portal::OpenPortal()
{
	this->open_ = true;
}

Portal::Portal(sf::Vector2f position, sf::Vector2f size) : Entity(position, size, object::Type::kPortal), open_(false), current_animation_(animation::Type::kMove)
{

}


Portal::~Portal()
{
	/*delete animations_[0];
	delete animations_[1];*/
}
