#pragma once
#include <map>
#include "Collidable.h"
#include "AnimationHolder.h"
#include "SpawnPoint.h"
#include "Coin.h"
#include "Portal.h"

namespace key {
	enum Type { kLeft, kRight, kDown, kUp, kAttack, kEnter }; //an enumerated type specifying the currently selected key on the keyboard
}
class Player :
	public Collidable
{
private:
	int health_, score_;									//variables storing the player's life and points
	static const float movement_speed_, gravity_force_;		//variables that store the player's movement and fall speed
	static const int maximal_jump_, jump_pause_, minimal_jump_; //variables storing the maximum and minimum height and the time_ of suspension in the air of the jump
	static const int starting_health_, invulnerability_length_; //variables storing starting health_ and the length of invulnerability after taking damage
	int jump_counter_, total_jump_counter_;					//variables storing the current jump time_ and the entire length of the current jump 
	int	invulnerability_counter_;							//a variable storing the current insensitivity length
	bool jumping_, falling_, on_ground_, crouching_, finished_level_; //flags indicating whether the player is jumping_, falling_, on the ground, crouching_ and has finished_ the level
	animation::Type current_animation_;						//a variable storing the current animation_ type
	AnimationHolder* animations_[7];						//a variable storing all player animations_
	sf::Vector2f current_spawn_position_;						//a variable storing the player's current spawn position
	sf::Vector2f  previous_position_;						//a variable holding the player's position a frame ago
	SpawnPoint* current_spawn_point_;							//a variable that stores a pointer to the player's current spawn



	void draw(sf::RenderTarget& target, sf::RenderStates state) const override; //a function that overwrites the library draw function
	void crouch();		//a function that supports player crouching_
	void stand();		//a function that supports getting up after the player crouches

public:
	Player(sf::Vector2f position, sf::Vector2f size);
	~Player();



	void ResolveCollision(Entity& other) override;

	void AddAnimation(animation::Type type, sf::Texture& texture, sf::Time duration, unsigned int frame_number, bool repeat);//a function that adds animations_ 
	void update(sf::Time dt, bool key_pressed[6]);	//a function that updates the player's movement
	void UpdateAnimation(sf::Time dt);				//a function that updates animations_

	void jump();			//a function that handles the player's jump
	bool IsJumping();		//a function that returns true when the player jumps
	void CheckGround();		//a function that returns true when the player is on the ground
	void AllowNextJump();	//a function that allows the player to make another jump

	int RemainingLives();	//a function that returns the player's current amount of life
	void hurt();			//a function that handles dealing damage to the player
	bool respawn();			//a function that handles player respawn

	bool HasMoved();		//a function that returns true when the player has moved since the previous frame
	int GetScore();
	void AddScore(int points); //a function that adds points to the player
	bool FinishedCurrentLevel();  //a function that returns true when the player has completed the level
};

