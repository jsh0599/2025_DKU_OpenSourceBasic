#ifndef CONTENTMENU_HPP
#define CONTENTMENU_HPP

#include "menustate.hpp"

class ContentMenu : public MenuState {
public:
    virtual ~ContentMenu() {}
    // 공통 메뉴 기능 예시
    virtual void handleSelection(int optionIndex) = 0;
};

#endif
