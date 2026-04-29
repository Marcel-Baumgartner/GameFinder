#pragma once

#include "GameFinderUI.h"
#include "GameItemUI.h"
#include "game.h"

#include <SFML/Graphics.hpp>

// std::vector<sf::Texture> loadCoverTextures(const std::vector<Game>& games, GameFinderUI& ui);
void createGameItems(const std::vector<Game>& games, GameFinderUI& ui, GameItemUI& gameItems);
void initUI(GameFinderUI& ui, GameItemUI& gameItems);
void handleEvents(sf::RenderWindow& window, GameFinderUI& ui, GameItemUI& gameItems, const std::string& apiKey);
void updateUI(GameFinderUI& ui, GameItemUI& gameItems);
void renderUI(sf::RenderWindow& window, GameFinderUI& ui, GameItemUI& gameItems);
