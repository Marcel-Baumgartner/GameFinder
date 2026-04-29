#pragma once

#include <SFML/Graphics.hpp>
#include <string>

struct GameFinderUI {

	// Textures
    sf::Texture backgroundTexture;
	sf::Texture searchBarTexture;

	// Sprites
    sf::Sprite backgroundSprite;
	sf::Sprite searchBarSprite;

	// Fonts
	sf::Font font;

	// Text
	sf::Text searchTextDrawable;

	// View
	sf::View view;

	// Parameters

	// Scrolling
	float scrollOffset = 0.f;

	// Search
	std::string searchText;
	bool searchActive = true;
	sf::RectangleShape searchBarHitbox;
	const float maxSearchWidth = 384.f;
};