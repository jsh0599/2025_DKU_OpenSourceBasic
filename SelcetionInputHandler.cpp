#include "MenuInputHelper.hpp"
#include "inputmanager.hpp"
#include "button.hpp"
#include "game.hpp"

void SelectionInputHandler::handle(InputManager* input, std::vector<Button*>& buttons, size_t& index,StateID& nextStateID) {
    
    while (input->pollAction()) {
        if (mInputManager->isGameExiting())
        {
            nextStateID = STATE_EXIT;
            break;
        }
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
