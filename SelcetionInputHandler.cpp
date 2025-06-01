#include "MenuInputHelper.hpp"
#include "inputmanager.hpp"
#include "button.hpp"
#include "game.hpp"

void SelcetionInputHandler::handle(InputManager* input, std::vector<Button*>& buttons, size_t& index) {
    while (input->pollAction()) {
        switch (input->getAction()) {
            case Action::move_up:
                if (index > 0) --index;
                break;
            case Action::move_down:
                if (index < buttons.size() - 1) ++index;
                break;
            case Action::select:
                buttons[index]->callbackFunction();
                break;
            case Action::back:
                Game::getInstance()->popState();
                break;
            default:
                break;
        }
    }
}
