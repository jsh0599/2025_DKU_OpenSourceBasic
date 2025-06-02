#include "abstractoptionstate.hpp"
#include "selectioninputhandler.hpp"
#include "game.hpp"

AbstractOptionState::AbstractOptionState(InputManager* inputManager)
    : State(inputManager), index(0) {}

AbstractOptionState::~AbstractOptionState() {
    exit();
}

void AbstractOptionState::exit() {
    for (auto btn : mButtons) delete btn;
    mButtons.clear();
}

void AbstractOptionState::run() {
    update();
    draw();
}

void AbstractOptionState::update() {
    SelectionInputHandler::handle(mInputManager, mButtons, index, nextStateID);
}

void AbstractOptionState::draw() {
    Game::getInstance()->mRenderer->clearScreen();

    for (auto btn : mButtons) btn->draw();
    SelectionInputHandler::renderHighlight(mButtons, index);

    drawOptions(); // 개별 설정 그리기

    Game::getInstance()->mRenderer->updateScreen();
}

