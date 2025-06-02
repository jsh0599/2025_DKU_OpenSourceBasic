#ifndef OPTIONSSTATE_HPP
#define OPTIONSSTATE_HPP

#include <vector>
#include "button.hpp"
#include "inputmanager.hpp"
#include "state.hpp"
#include "texture.hpp"

class OptionsState : public State {
public:
    OptionsState(InputManager* manager);
    virtual ~OptionsState();

    void run() override;
    void update() override;
    void exit() override;

protected:
    std::vector<Button*> mButtons;
    Texture* title_text;
    int index;

    // 순수 가상 함수: 하위 옵션 화면마다 다르게 구현
    virtual void initialize() override = 0;
    virtual void draw() override = 0;
    virtual void changeSetting(int selectedIndex, int direction) = 0;
};

#endif // OPTIONSSTATE_HPP
