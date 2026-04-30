#include "api.h"
#include "ui.h"
#include "game.h"
#include "GameFinderUI.h"
#include "GameItemUI.h"
#include "json.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <curl/curl.h>

using namespace std;
using json = nlohmann::json;

string loadApiKey() {
    ifstream file(".env");
    string line;

    while (getline(file, line)) {
        if (line.find("API_KEY=") == 0) {
            return line.substr(8); // Extract the API key
        }
    }
    return ""; // Return an empty string if the API key is not found
}

sf::Texture loadTextureFromUrl(const string& url) {
    string data = fetchUrl(url);

    sf::Texture texture;

    if (!data.empty()) {
        texture.loadFromMemory(data.data(), data.size());
    }

    return texture;
}

// Store the game data in a struct and return it
Game parseGame(const json& item) {
    Game game;

    // Strings
    if (item.contains("name") && item["name"].is_string()) {
        game.title = item["name"].get<string>();
    }
    else {
        game.title = "";
    }

    if (item.contains("released") && item["released"].is_string()) {
        game.releaseDate = item["released"].get<string>();
    }
    else {
        game.releaseDate = "";
    }

    // Metacritic
    if (item.contains("metacritic") && item["metacritic"].is_number()) {
        game.metacritic = static_cast<int>(item["metacritic"].get<double>());
    }
    else {
        game.metacritic = -1;
    }

    // Genres
    game.genres.clear();
    if (item.contains("genres") && item["genres"].is_array()) {
        for (const auto& g : item["genres"]) {
            if (g.contains("name") && g["name"].is_string()) {
                game.genres.push_back(g["name"].get<string>());
            }
        }
    }

    // Platforms
    game.platforms.clear();
    if (item.contains("platforms") && item["platforms"].is_array()) {
        for (const auto& p : item["platforms"]) {
            if (p.contains("platform") &&
                p["platform"].is_object() &&
                p["platform"].contains("name") &&
                p["platform"]["name"].is_string()) {

                game.platforms.push_back(
                    p["platform"]["name"].get<string>()
                );
            }
        }
    }

    // Cover image
    if (item.contains("background_image") &&
        item["background_image"].is_string() &&
        !item["background_image"].is_null()) {

        game.coverUrl = item["background_image"].get<string>();
    }
    else {
        game.coverUrl = "";
    }

    return game;
}

// Store the game data in a vector and return it
vector<Game> parseGames(const json& data) {
    vector<Game> games;

    if (data.contains("results")) {
        for (const auto& item : data["results"]) {
            games.push_back(parseGame(item));
        }
    }

    return games;
}

// Callback function to write the response data into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* s) {
    size_t newLength = size * nmemb;
    s->append((char*)contents, newLength);
    return newLength;
}

// Function to fetch data from a URL and return the response as a string
string fetchUrl(const string& url) {
    CURL* curl = curl_easy_init();
    string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
    }

    return response;
}

// Function to get games based on a search query and API key
vector<Game> getGames(const string& search, const string& apiKey) {

    string encodedSearch = search;

    replace(encodedSearch.begin(), encodedSearch.end(), ' ', '+');

    string url =
        "https://api.rawg.io/api/games?key=" +
        apiKey +
        "&search=" +
        encodedSearch;

    string response = fetchUrl(url);

    if (response.empty()) {
        return {};
    }

    json data = json::parse(response);

    return parseGames(data);
}

void loadGames(const string& query, const string& apiKey, GameFinderUI& ui, GameItemUI& gameItems) {

	// clear old games
    gameItems.coverTexture.clear();
    gameItems.coverSprite.clear();
    gameItems.titleText.clear();
    gameItems.releaseDateText.clear();
    gameItems.metacriticText.clear();
    gameItems.platformsText.clear();
    gameItems.genresText.clear();
    gameItems.coverFrame.clear();
    gameItems.itemFrame.clear();

    // load new games
    vector<Game> games = getGames(query, apiKey);

	// create new game items
    createGameItems(games, ui, gameItems);

    // initialize UI
    initUI(ui, gameItems);
}