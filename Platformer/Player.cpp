#include "Player.h"
#include <iostream>

const float Player::movement_speed_ = 150.0f;
const int Player::maximal_jump_ = 25;
const int Player::minimal_jump_ =10;
const int Player::jump_pause_ = 2;
const float Player::gravity_force_ = 250.f;
const int Player::starting_health_ = 5;
const int Player::invulnerability_length_ = 120;
void Player::draw(sf::RenderTarget & target, sf::RenderStates state) const
{
	if (animations_[current_animation_] != nullptr)
	{
		if (animations_[current_animation_]->GetDirection() == animation::Direction::kRight)
		{
			if (!crouching_)
				animations_[current_animation_]->SetPosition(sf::Vector2f{ obj_.getPosition().x - 13.f,obj_.getPosition().y - 6.5f });
			else
				animations_[current_animation_]->SetPosition(sf::Vector2f{ obj_.getPosition().x - 13.f,obj_.getPosition().y - 6.5f -obj_.getSize().y/2.f});
		}
		else
		{
			if (!crouching_)
				animations_[current_animation_]->SetPosition(sf::Vector2f{ obj_.getPosition().x + 13.f,obj_.getPosition().y - 6.5f });
			else
				animations_[current_animation_]->SetPosition(sf::Vector2f{ obj_.getPosition().x + 13.f,obj_.getPosition().y - 6.5f - obj_.getSize().y /2.f});
		}
		

		
		target.draw(*animations_[current_animation_], state);
		//target.Draw(obj_, state);
		
	}
	else
		target.draw(obj_, state);

	
}




Player::Player(sf::Vector2f position, sf::Vector2f size):

Collidable(position, size, object::Type::kPlayer), 
invulnerability_counter_(invulnerability_length_),
jumping_(false),
falling_(true),
crouching_(false),
finished_level_(false),
jump_counter_(0),
health_(starting_health_),
current_spawn_position_(position)
{
	obj_.setFillColor(sf::Color::Red);
}

Player::~Player()
{
	delete animations_[0];
	delete animations_[1];
	delete animations_[2];
	delete animations_[3];
	delete animations_[4];

}




void Player::ResolveCollision(Entity &other)
{
	collision::Type c_type=IsColliding(other,velocity_);
	if (c_type == collision::kNoCollision)
	{
		if(on_ground_!=true)
			falling_ = true;
		return;
	}
	
	switch (other.GetType())
	{
	case object::kWall:
		switch(c_type)
		{
		case collision::kLeft: SetXPosition(other.GetLeft() - GetSize().x / 2.0f);// std::cout << "kLeft" << std::endl;
			return;
		case collision::kRight: SetXPosition(other.GetRight() + GetSize().x / 2.0f);//std::cout << "kRight" << std::endl; 
			return;
		case collision::kTop: SetYPosition(other.GetTop() - GetSize().y / 2.0f); on_ground_ = true;
 			falling_ = false;// std::cout << "kTop" << std::endl;
			return;
		case collision::kBottom: SetYPosition(other.GetBottom() + GetSize().y / 2.0f); jump_counter_ = 0;// std::cout << "kBottom" << std::endl; 
			return;
		}
		break;
	case object::kSpikes:
		switch (c_type)
		{
		case collision::kLeft: SetXPosition(other.GetLeft() - GetSize().x / 2.0f); //this->hurt(); 
			return;
		case collision::kRight: SetXPosition(other.GetRight() + GetSize().x / 2.0f);  //this->hurt();
			return;
		case collision::kTop: this->hurt();
			return;
		case collision::kBottom: SetYPosition(other.GetBottom() + GetSize().y / 2.0f); jump_counter_ = 0;// std::cout << "kBottom" << std::endl; 
			return;
		}
		break;
	case object::kSpawnPoint: 
		if (!destruction_flag_)
		{
			if (current_spawn_point_ != nullptr && current_spawn_point_ != &dynamic_cast<SpawnPoint&>(other))
				current_spawn_point_->SetEnabled(false);
			current_spawn_point_ = &dynamic_cast<SpawnPoint&>(other);
			current_spawn_point_->SetEnabled(true);
			current_spawn_position_ = current_spawn_point_->GetPosition();
		}
		return;
		break;
	case object::kEnemy:
		this->hurt();
		return;
		break;
	case object::kCoin:
		score_ += dynamic_cast<Coin&>(other).GetValue();
		other.destroy();
		break;
	case object::kPortal:
		if (dynamic_cast<Portal&>(other).IsOpen())
			finished_level_ = true;
		break;
	}
	
}




void Player::AddAnimation(animation::Type type, sf::Texture & texture, sf::Time duration, unsigned int frame_number, bool repeat)
{
	if (animations_[type] != nullptr)
		delete(animations_[type]);
	animations_[type] = new AnimationHolder(texture, duration, frame_number, repeat);
	

}
void Player::update(sf::Time elapsedTime, bool key_pressed[6])
{
	if (invulnerability_counter_ > 0)
		invulnerability_counter_--;
	velocity_ = sf::Vector2f{ 0.f,0.f };
	previous_position_ = this->GetPosition();
	if (!destruction_flag_)
	{

		if (key_pressed[key::kLeft])
		{
			velocity_.x -= movement_speed_;
			invulnerability_counter_=0;
		}
		if (key_pressed[key::kRight])
		{
			velocity_.x += movement_speed_;
			invulnerability_counter_ = 0;
		}
		if (key_pressed[key::kDown])
		{
			this->crouch();
			invulnerability_counter_ = 0;
		}
		else
		{
			this->stand();
		}
		if (key_pressed[key::kUp])
		{
			this->jump();
			invulnerability_counter_ = 0;
		}
		else
			this->AllowNextJump();


		if (this->IsJumping())
		{
			velocity_.y -= gravity_force_*1.3f;
		}
		else if (falling_)
		{
			if (jump_counter_ == 0)
				velocity_.y += gravity_force_;
		}
	}
	else
		velocity_.y += gravity_force_;
	if (velocity_ != sf::Vector2f{ 0.f,0.f })
	{
		on_ground_ = false;
		velocity_ *= elapsedTime.asSeconds();
		this->obj_.move(velocity_);
	}

}
void Player::UpdateAnimation(sf::Time dt)
{
	sf::Vector2f diff_vector = this->GetPosition() - previous_position_;
	animation::Type next_animation;
	animation::Direction next_direction;
	if (destruction_flag_)
	{
		next_direction = animations_[current_animation_]->GetDirection();
		next_animation = animation::Type::kDeath;

	}
	else
	{
		if (diff_vector.x != 0)
		{
			if (on_ground_)
				next_animation = animation::Type::kMove;
			else
				next_animation = current_animation_;

			if (diff_vector.x > 0)
				next_direction = animation::Direction::kRight;
			else
				next_direction = animation::Direction::kLeft;
		}

		else if (current_animation_ != animation::Type::kJump)
			next_animation = animation::Type::kIdle;
		else
			next_animation = current_animation_;

		

		if (diff_vector.y > 0 && !on_ground_)
			next_animation = animation::Type::kFall;
		if (diff_vector.y < 0&& !on_ground_ &&velocity_.y<0)
			next_animation = animation::Type::kJump;
		if (crouching_)
			next_animation = animation::Type::kCrouch;
		if (diff_vector.x == 0)
			next_direction = animations_[current_animation_]->GetDirection();

		
	}

	if (next_animation != current_animation_)
		animations_[next_animation]->restart();
	current_animation_ = next_animation;
	
	

	if (animations_[current_animation_]->GetDirection() != next_direction)
	{
		animations_[current_animation_]->swap();
	}
	if (invulnerability_counter_ > 0)
		animations_[current_animation_]->SetColor(sf::Color::Yellow);
	else if(current_animation_==animation::kDeath)
		animations_[current_animation_]->SetColor(sf::Color::Red);
	else
		animations_[current_animation_]->SetColor(sf::Color::White);
	animations_[current_animation_]->update(dt);
}

void Player::crouch()
{
	if (on_ground_)
	{
		if (!crouching_)
		{
			crouching_ = true;
			this->SetYPosition(obj_.getPosition().y+obj_.getSize().y/3.f);
			obj_.setSize(sf::Vector2f{ obj_.getSize().x, obj_.getSize().y / 1.5f });

		}

		velocity_.x = 0;
	}
}

void Player::stand()
{
	if (crouching_)
	{
		crouching_ = false;
		obj_.setSize(sf::Vector2f{ obj_.getSize().x, obj_.getSize().y * 1.5f });
		this->SetYPosition(obj_.getPosition().y - obj_.getSize().y / 3.f);
	}
}


void Player::jump()
{
	if (!falling_&&!jumping_&&!crouching_)
	{
		jumping_ = true;
		jump_counter_ = minimal_jump_;
		total_jump_counter_ = jump_counter_;

	}
	else if (jump_counter_!=0)
	{
		if (total_jump_counter_ < maximal_jump_)
		{
			jump_counter_++;
			total_jump_counter_++;
			if (total_jump_counter_ > maximal_jump_)
			{
				jump_counter_ -= total_jump_counter_ - maximal_jump_;
				total_jump_counter_ = maximal_jump_;
			}
		}
	}
	return;
}

bool Player::IsJumping()
{
	if(jump_counter_>0)
	{
		if (--jump_counter_ > jump_pause_)
			return true;
	}
		return false;
}




void Player::CheckGround()
{
	on_ground_ = false;
}

void Player::AllowNextJump()
{
	jumping_ = false;
}


int Player::RemainingLives()
{
	return health_;
}

void Player::hurt()
{
	if (!this->destruction_flag_&&invulnerability_counter_==0)
	{
		health_--;
		this->stand();
		destruction_flag_ = true;
	}
}

bool Player::respawn()
{
	if (destruction_flag_ && animations_[animation::Type::kDeath]->IsFinished() && health_ > 0)
	{
		this->SetPosition(current_spawn_position_ + sf::Vector2f{0,this->GetSize().y/2});
		this->destruction_flag_ = false;
		this->falling_ = true;
		this->jump_counter_ = 0;
		this->on_ground_ = false;
		this->invulnerability_counter_ = invulnerability_length_;
	}
	if (health_ <= 0)
		return !animations_[animation::Type::kDeath]->IsFinished();
	else
		return true;
}

bool Player::HasMoved()
{
	return!(previous_position_ - this->GetPosition() == sf::Vector2f{ 0,0 })||falling_;
}

int Player::GetScore()
{
	return score_;
}

void Player::AddScore(int points)
{
	score_ += points;
}

bool Player::FinishedCurrentLevel()
{
	return finished_level_;
}

