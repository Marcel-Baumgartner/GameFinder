#include "ui.h"
#include "api.h"
#include "GameFinderUI.h"
#include "GameItemUI.h"
#include "game.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

void createGameItems(const vector<Game>& games, GameFinderUI& ui, GameItemUI& gameItems) {

	gameItems.coverTexture.reserve(games.size());
	gameItems.coverSprite.reserve(games.size());
	gameItems.titleText.reserve(games.size());
	gameItems.releaseDateText.reserve(games.size());
	gameItems.metacriticText.reserve(games.size());
	gameItems.platformsText.reserve(games.size());
	gameItems.genresText.reserve(games.size());

	int i = 0;

	for (const auto& game : games) {
		
		if (!game.coverUrl.empty()) {
			gameItems.coverTexture.push_back(loadTextureFromUrl(game.coverUrl));
		}
		else {
			sf::Texture tex;
			tex.loadFromFile("textures/placeholder.png");
			gameItems.coverTexture.push_back(tex);
		}

		gameItems.coverSprite.push_back(sf::Sprite());
		gameItems.coverSprite.back().setTexture(gameItems.coverTexture.back());
		gameItems.titleText.push_back(sf::Text());
		gameItems.titleText.back().setFont(ui.font);
		gameItems.titleText.back().setCharacterSize(14);
		gameItems.titleText.back().setString(game.title);
		gameItems.releaseDateText.push_back(sf::Text());
		gameItems.releaseDateText.back().setFont(ui.font);
		gameItems.releaseDateText.back().setCharacterSize(12);
		gameItems.releaseDateText.back().setString(game.releaseDate);

		if (game.metacritic != -1) {
			gameItems.metacriticText.push_back(sf::Text());
			gameItems.metacriticText.back().setFont(ui.font);
			gameItems.metacriticText.back().setString("Metacritic: " + to_string(game.metacritic));
			gameItems.metacriticText.back().setCharacterSize(12);
		}
		else {
			gameItems.metacriticText.push_back(sf::Text());
			gameItems.metacriticText.back().setFont(ui.font);
			gameItems.metacriticText.back().setString("Metacritic: N/A");
			gameItems.metacriticText.back().setCharacterSize(12);
		}

		string platforms = "Platforms: ";
		if (game.platforms.empty()) {
			platforms += "N/A";
		}
		for (const auto& p : game.platforms) {
			platforms += p + ", ";
		}
		if (!game.platforms.empty()) {
			platforms.pop_back(); // Remove last space
			platforms.pop_back(); // Remove last comma
		}
		gameItems.platformsText.push_back(sf::Text());
		gameItems.platformsText.back().setFont(ui.font);
		gameItems.platformsText.back().setCharacterSize(12);
		gameItems.platformsText.back().setString(platforms);

		string genres = "Genres: ";
		if (game.genres.empty()) {
			genres += "N/A";
		}
		for (const auto& g : game.genres) {
			genres += g + ", ";
		}
		if (!game.genres.empty()) {
			genres.pop_back(); // Remove last space
			genres.pop_back(); // Remove last comma
		}
		gameItems.genresText.push_back(sf::Text());
		gameItems.genresText.back().setFont(ui.font);
		gameItems.genresText.back().setCharacterSize(12);
		gameItems.genresText.back().setString(genres);

		i++;
	}
}

void initUI(GameFinderUI& ui, GameItemUI& gameItems) {

	// GameFinderUI

	// Textures
	ui.backgroundTexture.loadFromFile("textures/background.png");
	ui.searchBarTexture.loadFromFile("textures/searchbar.png");

	// Sprites
	ui.backgroundSprite.setTexture(ui.backgroundTexture);
	ui.searchBarSprite.setTexture(ui.searchBarTexture);

	// Fonts
	ui.font.loadFromFile("fonts/Readfine.ttf");
	
	// View
	ui.view.setSize(1280.f, 720.f);
	ui.view.setCenter(640.f, 360.f);

	// Texts
	ui.searchTextDrawable.setFont(ui.font);
	ui.searchTextDrawable.setFillColor(sf::Color::Black);
	ui.searchTextDrawable.setCharacterSize(24);
	ui.searchTextDrawable.setPosition(448, 16);

	// Search bar hitbox
	ui.searchBarHitbox.setSize(sf::Vector2f(400.f, 32.f));
	ui.searchBarHitbox.setPosition(440, 16);
	ui.searchBarHitbox.setFillColor(sf::Color::Transparent);

	// GameItemUI

	for (int i = 0; i < gameItems.coverTexture.size(); i++) {

		int x = (i % 4) * 320;
		int y = 64 +(i / 4) * 240;
		
		//Sprites
		auto texSize = gameItems.coverTexture[i].getSize();

		sf::Vector2f boxSize(128.f, 128.f);

		float scaleX = boxSize.x / texSize.x;
		float scaleY = boxSize.y / texSize.y;

		float scale = min(scaleX, scaleY);

		sf::FloatRect coverBounds = gameItems.coverSprite[i].getLocalBounds();

		gameItems.coverSprite[i].setOrigin(
			coverBounds.width / 2.f,
			coverBounds.height / 2.f
		);

		gameItems.coverSprite[i].setScale(scale, scale);

		sf::Vector2f boxPos(x + 96.f, y + 32.f);

		gameItems.coverSprite[i].setPosition(
			boxPos.x + boxSize.x / 2.f,
			boxPos.y + boxSize.y / 2.f
		);

		sf::RectangleShape frame;
		frame.setSize(boxSize);
		frame.setOrigin(boxSize.x / 2.f, boxSize.y / 2.f);
		frame.setPosition(boxPos.x + boxSize.x / 2.f, boxPos.y + boxSize.y / 2.f);
		frame.setFillColor(sf::Color::White);
		frame.setOutlineThickness(2.f);
		frame.setOutlineColor(sf::Color::Black);
		gameItems.coverFrame.push_back(frame);

		sf::RectangleShape itemFrame;
		itemFrame.setSize(sf::Vector2f(320.f, 240.f));
		itemFrame.setOrigin(160.f, 120.f);
		itemFrame.setPosition(x + 160.f, y + 120.f);
		itemFrame.setFillColor(sf::Color::Transparent);
		itemFrame.setOutlineThickness(2.f);
		itemFrame.setOutlineColor(sf::Color::White);
		gameItems.itemFrame.push_back(itemFrame);	

		// Texts

		sf::FloatRect titleBounds = gameItems.titleText[i].getLocalBounds();
		gameItems.titleText[i].setOrigin(
			titleBounds.left + titleBounds.width / 2.f,
			titleBounds.top + titleBounds.height / 2.f
		);
		gameItems.titleText[i].setPosition(x + 160.f, y + 16.f);

		sf::FloatRect releaseDateBounds = gameItems.releaseDateText[i].getLocalBounds();
		gameItems.releaseDateText[i].setOrigin(
			releaseDateBounds.left + releaseDateBounds.width / 2.f,
			releaseDateBounds.top + releaseDateBounds.height / 2.f
		);
		gameItems.releaseDateText[i].setPosition(x + 160.f, y + 168.f);

		sf::FloatRect metacriticBounds = gameItems.metacriticText[i].getLocalBounds();
		gameItems.metacriticText[i].setOrigin(
			metacriticBounds.left + metacriticBounds.width / 2.f,
			metacriticBounds.top + metacriticBounds.height / 2.f
		);
		gameItems.metacriticText[i].setPosition(x + 160.f, y + 184.f);

		sf::FloatRect platformsBounds = gameItems.platformsText[i].getLocalBounds();
		gameItems.platformsText[i].setOrigin(
			platformsBounds.left + platformsBounds.width / 2.f,
			platformsBounds.top + platformsBounds.height / 2.f
		);
		gameItems.platformsText[i].setPosition(x + 160.f, y + 200.f);

		sf::FloatRect genresBounds = gameItems.genresText[i].getLocalBounds();
		gameItems.genresText[i].setOrigin(
			genresBounds.left + genresBounds.width / 2.f,
			genresBounds.top + genresBounds.height / 2.f
		);
		gameItems.genresText[i].setPosition(x + 160.f, y + 216.f);
	}
}

void handleEvents(sf::RenderWindow& window, GameFinderUI& ui, GameItemUI& gameItems, const string& apiKey) {

	sf::Event event;

	while (window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) {
			window.close();
		}

		// Scrolling
		if (event.type == sf::Event::MouseWheelScrolled) {
			ui.scrollOffset -= event.mouseWheelScroll.delta * 30.f;
		}

		// Click on search bar
		if (event.type == sf::Event::MouseButtonPressed) {
			if (ui.searchBarHitbox.getGlobalBounds().contains(
				window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y })
			)) {
				ui.searchActive = true;
			}
			else {
				ui.searchActive = false;
			}
		}

		// Search input
		if (event.type == sf::Event::TextEntered && ui.searchActive) {

			sf::Uint32 unicode = event.text.unicode;

			// ENTER
			if (unicode == 13) {

				loadGames(ui.searchText, apiKey, ui, gameItems);

				continue;
			}

			// BACKSPACE
			if (unicode == 8) {
				if (!ui.searchText.empty())
					ui.searchText.pop_back();
			}

			// CHARS
			else if (unicode >= 32 && unicode < 127) {

				string newText = ui.searchText + static_cast<char>(unicode);

				ui.searchTextDrawable.setString(newText);

				if (ui.searchTextDrawable.getLocalBounds().width <= ui.maxSearchWidth) {
					ui.searchText = newText;
				}
			}
		}
	}
}

void updateUI(GameFinderUI& ui, GameItemUI& gameItems) {

	// Scrolling limits
	int rows = (gameItems.coverSprite.size() + 3) / 4;
	float contentHeight = rows * 240.f;

	float maxScroll = max(0.f, contentHeight - 656.f);

	if (ui.scrollOffset < 0.f)
		ui.scrollOffset = 0.f;

	if (ui.scrollOffset > maxScroll)
		ui.scrollOffset = maxScroll;

	ui.view.setCenter(640.f, 360.f + ui.scrollOffset);

	// Update search text
	ui.searchTextDrawable.setString(ui.searchText);
}

void renderUI(sf::RenderWindow& window, GameFinderUI& ui, GameItemUI& gameItems) {
	
	window.clear();

	window.setView(window.getDefaultView());
	window.draw(ui.backgroundSprite);
	
	window.setView(ui.view);
	for (int i = 0; i < gameItems.coverSprite.size(); i++) {
		window.draw(gameItems.coverFrame[i]);
		window.draw(gameItems.coverSprite[i]);
		window.draw(gameItems.titleText[i]);
		window.draw(gameItems.releaseDateText[i]);
		window.draw(gameItems.metacriticText[i]);
		window.draw(gameItems.platformsText[i]);
		window.draw(gameItems.genresText[i]);
		window.draw(gameItems.itemFrame[i]);
	}

	window.setView(window.getDefaultView());
	window.draw(ui.searchBarSprite);
	window.draw(ui.searchTextDrawable);
	window.display();
}