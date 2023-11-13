#pragma once
#include <map>
#include <SFML/Graphics.hpp>
namespace texture {
	enum Id { kPlayerIdle, kPlayerMove, kPlayerJump, kPlayerFall, kPlayerDie, kPlayerCrouch, kPlatform, kSpikes, kSpawnPoint, kBat, kBat2, kCoin, kPortalMove, kPortalIdle, kBackground };
																	//enumeration type storing the id of a given texture
}
class TextureHolder
{
private:
	std::map<texture::Id, std::unique_ptr<sf::Texture>> texture_map_;	//a map storing all textures_ in the game with their corresponding ids
public:
	void load(texture::Id id, const std::string& filename);	//a function that adds texture to the map
	sf::Texture& get(texture::Id id);							//function returning a texture with a given id
	const sf::Texture& get(texture::Id id) const;				//static reload of the above function
};

