#include "modeselectstate.hpp"
#include "game.hpp"
#include "config.hpp"
#include <iostream>
#include "content1menustate.hpp"
#include "content2menustate.hpp"
#include "content3menustate.hpp"

ModeSelectState::ModeSelectState(InputManager* manager)
    : MenuState(manager), descriptionTexture(nullptr) {}

void ModeSelectState::initialize() {
    index = 0;

    title_text = new Texture();
    title_text->loadFromText("TETRIS", Game::getInstance()->mRenderer->bigFont, config::default_text_color);

    descriptionTexture = new Texture();
    descriptionTexture->loadFromText("Select Mode", Game::getInstance()->mRenderer->mediumFont, config::default_text_color);

    int centerX = (config::logical_window_width - 200) / 2;

    addButton(new Button(
        "Single Mode",
        []() {
            std::cout << "Single Mode Started!" << std::endl;
            Game::getInstance()->pushNewState<GameState>();
        },
        centerX, 150, 200, 40
    ));

    addButton(new Button(
        "Multi Mode",
        []() {
            std::cout << "Multi Mode Started!" << std::endl;
            Game::getInstance()->pushNewState<MultiState>();
        },
        centerX, 200, 200, 40
    ));

    addButton(new Button(
        "Challenge Mode",
        []() {
            std::cout << "Challenge Mode Started!" << std::endl;
            Game::getInstance()->pushNewState<ChallengeMenuState>();
        },
        centerX, 250, 200, 40
    ));

    addButton(new Button(
        "Back",
        []() {
            Game::getInstance()->popState();
        },
        centerX, 300, 200, 40
    ));
}

void ModeSelectState::draw() {
    Game::getInstance()->mRenderer->clearScreen();

    if (title_text)
        title_text->renderCentered(config::logical_window_width / 2, 50);

    if (descriptionTexture)
        descriptionTexture->renderCentered(config::logical_window_width / 2, 90);

    for (auto button : mButtons)
        button->draw();

    SelectionInputHandler::renderHighlight(mButtons, index);

    Game::getInstance()->mRenderer->updateScreen();
}
