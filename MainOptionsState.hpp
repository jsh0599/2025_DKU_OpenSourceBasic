#ifndef MAINMENUOPTIONSTATE_HPP
#define MAINMENUOPTIONSTATE_HPP

#include "abstractoptionstate.hpp"
#include "texture.hpp"

class MainMenuOptionState : public AbstractOptionState {
    enum class SettingChange { left, right };

public:
    MainMenuOptionState(InputManager* inputManager);
    ~MainMenuOptionState();

    void initialize() override;
    void drawOptions() override;
    void update() override;

private:
    Button* OKButton;
    Texture* title_text;

    Texture* resolution_setting_text;
    Texture* resolution_text;
    Texture* ghost_block_setting_text;

    Texture* left_arrow;
    Texture* right_arrow;

    Texture* texture_on_on;
    Texture* texture_on_off;
    Texture* texture_off_on;
    Texture* texture_off_off;

    int resolution_scaling_index;

    void changeResolution(SettingChange s);
    void changeGhostBlock(SettingChange s);
};

#endif // MAINMENUOPTIONSTATE_HPP
