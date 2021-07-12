#include "texture_bank.h"


std::map<std::string, sf::Texture> Texture_bank::texture_map = std::map<std::string, sf::Texture>();

const sf::Texture* Texture_bank::getTexture(const std::string& key)
{
	auto it = texture_map.find(key);
	if (texture_map.find(key) != texture_map.end())
	{
		return &it->second; 
	}
	else
	{
		sf::Texture tex;
		if (!tex.loadFromFile(key))
		{
			//throw;
		}
		texture_map.insert({ key, tex });

		return &texture_map[key];
	}
}