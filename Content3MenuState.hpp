#ifndef CONTENT1MENUSTATE_HPP
#define CONTENT1MENUSTATE_HPP

#include "menustate.hpp"
#include "texture.hpp"

class Content3MenuState : public MenuState {
public:
    Content3MenuState(InputManager* manager);
    void initialize() override;
    void draw() override;

private:
    Texture* title_text = nullptr;
};

#endif
