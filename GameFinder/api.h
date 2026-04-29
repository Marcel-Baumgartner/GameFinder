#pragma once

#include "game.h"
#include "GameFinderUI.h"
#include "GameItemUI.h"
#include "json.hpp"

#include <string>
#include <vector>

std::string loadApiKey();
Game parseGame(const nlohmann::json& item);
sf::Texture loadTextureFromUrl(const std::string& url);
std::vector<Game> parseGames(const nlohmann::json& data);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);
std::string fetchUrl(const std::string& url);
std::vector<Game> getGames(const std::string& search, const std::string& apiKey);
void loadGames(const std::string& query, const std::string& apiKey, GameFinderUI& ui, GameItemUI& gameItems);