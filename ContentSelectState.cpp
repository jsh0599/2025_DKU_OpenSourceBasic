#include "ContentSelectState.hpp"
#include "game.hpp"
#include "config.hpp"
#include <iostream>

ContentSelectState::ContentSelectState(InputManager* manager)
    : MenuState(manager), descriptionTexture(nullptr) {}

void ContentSelectState::initialize() {
    index = 0;

    title_text = new Texture();
    title_text->loadFromText("TETRIS", Game::getInstance()->mRenderer->bigFont, config::default_text_color);

    descriptionTexture = new Texture();
    descriptionTexture->loadFromText("Select Content", Game::getInstance()->mRenderer->mediumFont, config::default_text_color);

    int centerX = (config::logical_window_width - 200) / 2;

    addButton(new Button("Content 1", &ContentSelectState::startContent1, centerX, 150, 200, 40));
    addButton(new Button("Content 2", &ContentSelectState::startContent2, centerX, 200, 200, 40));
    addButton(new Button("Content 3", &ContentSelectState::startContent3, centerX, 250, 200, 40));
    addButton(new Button("Back", &ContentSelectState::goBack, centerX, 300, 200, 40));
}

void ContentSelectState::draw() {
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

// 콜백 함수들
void ContentSelectState::startContent1() {
    std::cout << "Content 1 Started!" << std::endl;
    Game::getInstance()->pushNewGame();
}

void ContentSelectState::startContent2() {
    std::cout << "Content 2 Started!" << std::endl;
    Game::getInstance()->pushMulti();
}

void ContentSelectState::startContent3() {
    std::cout << "Content 3 Started!" << std::endl;
    Game::pushChallengeMenu();
}

void ContentSelectState::goBack() {
    Game::getInstance()->popState();
}
