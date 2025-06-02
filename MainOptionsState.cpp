#include "mainoptionsstate.hpp"
#include "game.hpp"
#include "config.hpp"
#include <iostream>

MainOptionsState::MainOptionsState(InputManager* manager)
    : OptionsState(manager), resolutionText(nullptr), ghostBlockText(nullptr), descriptionText(nullptr) {}

void MainOptionsState::initialize() {
    index = 0;

    title_text = new Texture();
    title_text->loadFromText("Options", Game::getInstance()->mRenderer->bigFont, config::default_text_color);

    resolutionText = new Texture();
    ghostBlockText = new Texture();
    descriptionText = new Texture();

    for (int i = 0; i < config::possible_resolution_scalings; i++) {
        if (config::resolution_scaling == config::available_resolution_scalings[i]) {
            resolutionIndex = i;
        }
    }

    mButtons.push_back(new Button("Resolution", nullptr, 50, 120, 200, 40));
    mButtons.push_back(new Button("Ghost Block", nullptr, 50, 200, 200, 40));
    mButtons.push_back(new Button("Back", &Game::goBack, 50, 280, 200, 40));
}

void MainOptionsState::draw() {
    Game::getInstance()->mRenderer->clearScreen();

    if (title_text)
        title_text->renderCentered(config::logical_window_width / 2, 50);

    resolutionText->loadFromText(
        "Resolution: " + std::to_string(int(config::logical_window_width * config::resolution_scaling)) + "x" +
        std::to_string(int(config::logical_window_height * config::resolution_scaling)),
        Game::getInstance()->mRenderer->mediumFont,
        config::default_text_color
    );
    resolutionText->render(300, 130);

    ghostBlockText->loadFromText(
        std::string("Ghost Block: ") + (config::ghost_piece_enabled ? "ON" : "OFF"),
        Game::getInstance()->mRenderer->mediumFont,
        config::default_text_color
    );
    ghostBlockText->render(300, 210);

    for (auto btn : mButtons) btn->draw();
    SelectionInputHandler::renderHighlight(mButtons, index);

    Game::getInstance()->mRenderer->updateScreen();
}

void MainOptionsState::changeSetting(int selectedIndex, int direction) {
    if (selectedIndex == 0) {
        // Resolution 변경
        resolutionIndex += direction;
        if (resolutionIndex < 0) resolutionIndex = 0;
        if (resolutionIndex >= config::possible_resolution_scalings)
            resolutionIndex = config::possible_resolution_scalings - 1;

        config::resolution_scaling = config::available_resolution_scalings[resolutionIndex];
        SDL_SetWindowSize(Game::getInstance()->mWindow,
                          config::logical_window_width * config::resolution_scaling,
                          config::logical_window_height * config::resolution_scaling);
        SDL_SetWindowPosition(Game::getInstance()->mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
    else if (selectedIndex == 1) {
        // Ghost Block 토글
        config::ghost_piece_enabled = !config::ghost_piece_enabled;
    }
}
