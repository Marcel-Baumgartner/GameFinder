#include "api.h"
#include "ui.h"
#include "GameFinderUI.h"
#include "GameItemUI.h"
#include "game.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace std;

int main() {

    string apiKey = loadApiKey();

    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "GameFinder");

    window.setKeyRepeatEnabled(true);

    GameFinderUI ui;
    GameItemUI gameItems;
    
    initUI(ui, gameItems);

    while (window.isOpen()) {
        handleEvents(window, ui, gameItems, apiKey);
        updateUI(ui, gameItems);
        renderUI(window, ui, gameItems);
    }

    return 0;
}