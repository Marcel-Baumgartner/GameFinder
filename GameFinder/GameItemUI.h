#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

struct GameItemUI {
	
	// Textures
	std::vector<sf::Texture> coverTexture;

	//Sprites
	std::vector<sf::Sprite> coverSprite;
	std::vector<sf::RectangleShape> coverFrame;
	std::vector<sf::RectangleShape> itemFrame;

	// Texts
	std::vector<sf::Text> titleText;
	std::vector<sf::Text> releaseDateText;
	std::vector<sf::Text> metacriticText;
	std::vector<sf::Text> platformsText;
	std::vector<sf::Text> genresText;
};