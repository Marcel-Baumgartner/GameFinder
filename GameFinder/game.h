#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

struct Game {
	std::string title;
	std::string releaseDate;
	int metacritic = -1;
	std::vector<std::string> platforms;
	std::vector<std::string> genres;
	std::string coverUrl;
};
