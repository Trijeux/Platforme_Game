#include "TextureHolder.h"



void TextureHolder::load(texture::Id id, const std::string & filename)
{
	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	if (!texture->loadFromFile(filename))
		throw ("Error loading texture "+id);
	texture->setRepeated(true);
	texture_map_.insert(std::make_pair(id,std::move(texture)));
}

const sf::Texture & TextureHolder::get(texture::Id id) const
{
	auto result = texture_map_.find(id);
	return *result->second;
}
sf::Texture& TextureHolder::get(texture::Id id)
{
	auto found = texture_map_.find(id);   
	return *found->second; 
}




