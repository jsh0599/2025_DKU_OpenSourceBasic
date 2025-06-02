#ifndef CONTENTSELECTSTATE_HPP
#define CONTENTSELECTSTATE_HPP

#include "menustate.hpp"
#include "button.hpp"
#include "texture.hpp"

class ContentSelectState : public MenuState {
public:
    ContentSelectState(InputManager* manager);

    void initialize() override;
    void draw() override;

private:
    Texture* descriptionTexture;

    static void startMode1();
    static void startMode2();
    static void startMode3();
    static void goBack();
};

#endif // CONTENTSELECTSTATE_HPP
