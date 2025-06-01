#include "game.hpp"

#include "menustate.hpp"
#include "optionsstate.hpp"
#include "pausedstate.hpp"
#include "gamestate.hpp"
#include "SpeedChallengeState.hpp"
#include "ModeSelectState.hpp"
#include "MultiState.hpp"
#include "ChallengeMenuState.hpp"

#include "state_factory.hpp"

Game *Game::mInstance = nullptr;

Game::Game()
{
    mRenderer = nullptr;
    mWindow = nullptr;
    mManager = new InputManager();
}

Game *Game::getInstance()
{
    if (mInstance == nullptr)
        mInstance = new Game();

    return mInstance;
}

bool Game::initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        return false;

    int actual_width = config::logical_window_width * config::resolution_scaling;
    int actual_height = config::logical_window_height * config::resolution_scaling;

    mWindow = SDL_CreateWindow("PixelTetris", 
                               SDL_WINDOWPOS_CENTERED, 
                               SDL_WINDOWPOS_CENTERED,
                               actual_width, 
                               actual_height, 
                               SDL_WINDOW_SHOWN);

    if (mWindow == nullptr)
        return false;

    if (!mRenderer)
        mRenderer = new Renderer();

    mRenderer->initialize(mWindow);

    if (TTF_Init() == -1)
        return false;

    pushNewState<MenuState>();

    return true;
}



void Game::run() {
    // 이전 프레임에서 pop된 상태 안전하게 삭제
    if (mPendingDeleteState) {
        delete mPendingDeleteState;
        mPendingDeleteState = nullptr;
    }

    if (!mStates.empty()) {
        mStates.back()->run();
    }
}

void Game::exit() {
    for (State* s : mStates) {
        delete s;
    }
    mStates.clear();

    // 만약 삭제 예약된 게 남아있다면
    if (mPendingDeleteState) {
        delete mPendingDeleteState;
        mPendingDeleteState = nullptr;
    }

    // Renderer, InputManager, SDL 자원도 해제
    delete mRenderer;
    delete mManager;

    SDL_DestroyWindow(mWindow);
    SDL_Quit();
    TTF_Quit();
}


void Game::popState() {
    if (!mStates.empty()) {
        // 삭제 예약만 하고 당장 delete 하지 않음
        mPendingDeleteState = mStates.back();
        mStates.pop_back();
    }
}

void Game::pushState(State *s)
{
    mStates.push_back(s);
}

void Game::changeState(State *s)
{
    if (!mStates.empty())
        popState();
    pushState(s);
}

template<typename T>
void Game::pushNewState()
{
    State* state = StateFactory::createState<T>(mManager);
    state->initialize();
    pushState(state);
}

void Game::pushOptions() { pushNewState<OptionsState>(); }
void Game::pushPaused() { pushNewState<PausedState>(); }
void Game::pushNewGame() { pushNewState<GameState>(); }
void Game::pushSpeedChallenge() { pushNewState<SpeedChallengeState>(); }
void Game::pushModeSelect() { pushNewState<ModeSelectState>(); }
void Game::pushChallengeMenu() { pushNewState<ChallengeMenuState>(); }
void Game::pushMulti() { pushNewState<MultiState>(); }

void Game::goBack() { popState(); }
void Game::goDoubleBack() 
{
    popState(); 
    popState(); 

}

bool Game::isGameExiting()
{
    return mStates.empty() || mStates.back()->nextStateID == STATE_EXIT;
}

   
