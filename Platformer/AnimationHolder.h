#pragma once
#include <SFML/Graphics.hpp>
namespace animation
{
	enum Direction { kLeft, kRight };						//enumerated type specifying which direction the animation_ is facing
	enum Type { kIdle, kMove, kJump, kFall, kDeath, kHit, kCrouch };	//an enumerated type specifying the animation_ type
}
class AnimationHolder :
	public sf::Drawable,
	public sf::Transformable
{
private:
	sf::Sprite m_sprite_;						//a variable that stores the current animation_ frame and texture and is displayed on the screen
	sf::Vector2u frame_size_;					//a variable that stores the size of one animation_ frame
	std::size_t frame_number_;				//a variable that stores the number of frames in the animation_
	std::size_t current_frame_;					//a variable that stores the number of the current frame of the animation_
	sf::Time anim_duration_;					//a variable that stores the duration of the entire animation_
	sf::Time elapsed_time_;					//a variable that stores how much time_ has passed since the beginning of the animation_
	animation::Direction anim_direction_;		//a variable that stores the page the animation_ is facing

	bool repeat_, finished_;					//a variable that stores whether the animation_ should be repeated and whether it has already ended
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override; //a function that overwrites the library draw function

public:
	AnimationHolder(sf::Texture& texture, sf::Time duration, unsigned int frame_number, bool repeat);


	void SetColor(sf::Color color);
	void SetPosition(sf::Vector2f pos);
	void update(sf::Time dt);				//a function that updates animations_ depending on how much time_ has passed
	void swap();							//a function that changes the turn of the animation_
	void restart();							//a function that restarts animations_
	bool IsFinished();

	animation::Direction GetDirection();
};

