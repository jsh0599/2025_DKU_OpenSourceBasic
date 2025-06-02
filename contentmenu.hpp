#ifndef CONTENTMENU_HPP
#define CONTENTMENU_HPP

#include "menustate.hpp"

class ContentMenu : public MenuState {
public:

    ContentMenu(InputManager* manager)

    void initialize() override;
    void draw() override;

};

#endif
