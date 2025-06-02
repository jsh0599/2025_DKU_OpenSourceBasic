#ifndef MAINOPTIONSSTATE_HPP
#define MAINOPTIONSSTATE_HPP

#include "optionsstate.hpp"

class MainOptionsState : public OptionsState {
public:
    MainOptionsState(InputManager* manager);
    void initialize() override;
    void draw() override;
    void changeSetting(int selectedIndex, int direction) override;

private:
    Texture* resolutionText;
    Texture* ghostBlockText;
    Texture* descriptionText;

    int resolutionIndex;
};

#endif // MAINOPTIONSSTATE_HPP
