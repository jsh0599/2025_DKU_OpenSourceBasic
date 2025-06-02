#ifndef ABSTRACTOPTIONSTATE_HPP
#define ABSTRACTOPTIONSTATE_HPP

#include <vector>
#include "button.hpp"
#include "inputmanager.hpp"
#include "state.hpp"

class AbstractOptionState : public State {
public:
    AbstractOptionState(InputManager* inputManager);
    virtual ~AbstractOptionState();

    void initialize() override = 0; // 반드시 서브클래스에서 구현
    void exit() override;

    void run() override;
    void update() override;
    void draw() override;

protected:
    int index;
    std::vector<Button*> mButtons;

    virtual void drawOptions() = 0; // 각 옵션 항목은 서브클래스에서 정의
};

#endif // ABSTRACTOPTIONSTATE_HPP
