#include "optionsstate.hpp"

#include <string>
#include "config.hpp"
#include "game.hpp"
#include "selectioninputhandler.hpp"
#include "button.hpp"

OptionsState::OptionsState(InputManager *manager) : State(manager) {}

OptionsState::~OptionsState() {
    exit();
}

void OptionsState::initialize() {
    index = 0;
    for (int i = 0; i < config::possible_resolution_scalings; i++) {
        if (config::resolution_scaling == config::available_resolution_scalings[i]) {
            resolution_scaling_index = i;
        }
    }

    title_text = new Texture();
    title_text->loadFromText("Options", Game::getInstance()->mRenderer->bigFont, config::default_text_color);

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    OKButton = new Button("../../assets/button-ok.png", &Game::goBack, (config::logical_window_width - 80) / 2, 280);
    #else
    OKButton = new Button("../assets/button-ok.png", &Game::goBack, (config::logical_window_width - 80) / 2, 280);
    #endif

    mButtons.clear();
    mButtons.push_back(new Button("Resolution", nullptr, 50, 100, 200, 40));
    mButtons.push_back(new Button("Ghost Block", nullptr, 50, 180, 200, 40));
    mButtons.push_back(OKButton);

    resolution_text = new Texture();
    resolution_setting_text = new Texture();
    ghost_block_setting_text = new Texture();

    left_arrow = new Texture();
    right_arrow = new Texture();
    texture_on_on = new Texture();
    texture_on_off = new Texture();
    texture_off_on = new Texture();
    texture_off_off = new Texture();

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    left_arrow->loadFromImage("../../assets/arrow-left.png");
    right_arrow->loadFromImage("../../assets/arrow-right.png");
    texture_on_on->loadFromImage("../../assets/button-on-on.png");
    texture_on_off->loadFromImage("../../assets/button-on-off.png");
    texture_off_on->loadFromImage("../../assets/button-off-on.png");
    texture_off_off->loadFromImage("../../assets/button-off-off.png");
    #else
    left_arrow->loadFromImage("../assets/arrow-left.png");
    right_arrow->loadFromImage("../assets/arrow-right.png");
    texture_on_on->loadFromImage("../assets/button-on-on.png");
    texture_on_off->loadFromImage("../assets/button-on-off.png");
    texture_off_on->loadFromImage("../assets/button-off-on.png");
    texture_off_off->loadFromImage("../assets/button-off-off.png");
    #endif
}

void OptionsState::exit() {
    delete title_text;
    delete resolution_text;
    delete resolution_setting_text;
    delete ghost_block_setting_text;
    delete texture_on_on;
    delete texture_on_off;
    delete texture_off_on;
    delete texture_off_off;
    delete OKButton;
    for (auto btn : mButtons) delete btn;
    mButtons.clear();
}

void OptionsState::run() {
    update();
    draw();
}

void OptionsState::update() {
    SelectionInputHandler::handle(mInputManager, mButtons, index);

    while (mInputManager->pollAction()) {
        switch (mInputManager->getAction()) {
            case Action::move_left:
                if (index == 0) changeResolution(SettingChange::left);
                else if (index == 1) changeGhostBlock(SettingChange::left);
                break;
            case Action::move_right:
                if (index == 0) changeResolution(SettingChange::right);
                else if (index == 1) changeGhostBlock(SettingChange::right);
                break;
            default:
                break;
        }
    }
}

void OptionsState::draw() {
    Game::getInstance()->mRenderer->clearScreen();
    title_text->renderCentered(config::logical_window_width / 2, 50);

    resolution_setting_text->loadFromText("Resolution", Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    resolution_setting_text->render(50, 100);
    ghost_block_setting_text->loadFromText("Ghost Block", Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    ghost_block_setting_text->render(50, 180);

    std::string resolution_string = std::to_string(int(config::logical_window_width * config::resolution_scaling)) + " x " +
        std::to_string(int(config::logical_window_height * config::resolution_scaling));
    resolution_text->loadFromText(resolution_string, Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    resolution_text->render(400 + (200 - resolution_text->getWidth()) / 2, 101);

    left_arrow->render(383, 105);
    right_arrow->render(590, 105);

    if (config::ghost_piece_enabled) {
        texture_off_off->render(400, 188);
        texture_on_on->render(510, 188);
    } else {
        texture_off_on->render(400, 188);
        texture_on_off->render(510, 188);
    }

    for (auto btn : mButtons) btn->draw();

    SelectionInputHandler::renderHighlight(mButtons, index)

    Game::getInstance()->mRenderer->updateScreen();
}

void OptionsState::changeResolution(SettingChange s) {
    if (s == SettingChange::left && resolution_scaling_index > 0) {
        --resolution_scaling_index;
    } else if (s == SettingChange::right && resolution_scaling_index < config::possible_resolution_scalings - 1) {
        ++resolution_scaling_index;
    }
    config::resolution_scaling = config::available_resolution_scalings[resolution_scaling_index];
    SDL_SetWindowSize(Game::getInstance()->mWindow,
        config::logical_window_width * config::resolution_scaling,
        config::logical_window_height * config::resolution_scaling);
    SDL_SetWindowPosition(Game::getInstance()->mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void OptionsState::changeGhostBlock(SettingChange s) {
    if ((s == SettingChange::left && config::ghost_piece_enabled) ||
        (s == SettingChange::right && !config::ghost_piece_enabled)) {
        config::ghost_piece_enabled = !config::ghost_piece_enabled;
    }
}
