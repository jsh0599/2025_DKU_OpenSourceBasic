#include "optionsstate.hpp"
#include "selectioninputhandler.hpp"
#include "game.hpp"

OptionsState::OptionsState(InputManager* manager)
    : State(manager), title_text(nullptr), index(0) {}

OptionsState::~OptionsState() {
    exit();
}

void OptionsState::run() {
    update();
    draw();
}

void OptionsState::update() {
    SelectionInputHandler::handle(mInputManager, mButtons, index, nextStateID);

    while (mInputManager->pollAction()) {
        switch (mInputManager->getAction()) {
            case Action::move_left:
                changeSetting(index, -1);
                break;
            case Action::move_right:
                changeSetting(index, +1);
                break;
            default:
                break;
        }
    }
}

void OptionsState::exit() {
    for (auto btn : mButtons) delete btn;
    mButtons.clear();
    if (title_text) {
        delete title_text;
        title_text = nullptr;
    }
}
