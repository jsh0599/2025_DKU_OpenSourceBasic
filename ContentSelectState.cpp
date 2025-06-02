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
    addButton(new Button("Single Mode", &ContentSelectState::startMode1, centerX, 150, 200, 40));
    addButton(new Button("Multi Mode", &ContentSelectState::startMode2, centerX, 200, 200, 40));
    addButton(new Button("Challenge Mode", &ContentSelectState::startMode3, centerX, 250, 200, 40));
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
void ContentSelectState::startMode1() {
    std::cout << "Single Mode Started!" << std::endl;
    Game::getInstance()->pushNewGame();
}

void ContentSelectState::startMode2() {
    std::cout << "Multi Mode Started!" << std::endl;
    Game::getInstance()->pushMulti();
}

void ContentSelectState::startMode3() {
    std::cout << "Challenge Mode Started!" << std::endl;
    Game::pushChallengeMenu();
}

void ContentSelectState::goBack() {
    Game::getInstance()->popState();
}
