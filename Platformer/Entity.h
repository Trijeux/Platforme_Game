#pragma once
#include <SFML/Graphics.hpp>

namespace object
{
	enum Type { kWall, kPlayer, kCoin, kSpawnPoint, kSpikes, kEnemy, kPortal };		//an enumeration type that specifies the type of the object
}

class Entity : public sf::Drawable
{
protected:

	sf::RectangleShape obj_;		//a variable that stores the rectangle that limits the object's collisions, and the texture in the case of objects without animation_
	bool destruction_flag_;		//a variable that stores the value_ of a flag specifying whether the object is destroyed or not
	object::Type obj_type_;		//a variable that stores the type of the object
private:
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;//a function that overwrites the library draw function
public:
	Entity(sf::Vector2f position, sf::Vector2f size, object::Type obj_type);
	void SetPosition(sf::Vector2f position);
	void SetXPosition(float x);
	void SetYPosition(float y);
	bool IsDestroyed();
	void destroy();												//a function that sets the destruction flag to true
	object::Type GetType();
	sf::Vector2f GetPosition();
	sf::FloatRect GetBoundingRect();							//a function that returns the scope of the rectangle limiting the object's collisions
	sf::RectangleShape GetRectangle();							//a function that returns a rectangle that limits the object's collisions
	float GetLeft();											//a function that returns the left edge of an object
	float GetRight();											//a function that returns the right edge of an object
	float GetTop();												//a function that returns the top edge of an object
	float GetBottom();											//a function that returns the bottom edge of an object
	sf::Vector2f GetSize();
	sf::Vector2f GetOrigin();									//a function that returns the center of an object
	void SetTexture(sf::Texture& texture, bool repeated);		//a function that sets the texture of an object
	void SetColor(sf::Color color);							//a function that sets the texture color of an object
	bool OnScreen(sf::View current_view);							//a function that returns true when an object or part of it is on the screen
};


