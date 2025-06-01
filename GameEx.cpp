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

void Game::exit()
{
    delete mRenderer;
    delete mManager;

    for (State *s : mStates)
        delete s;

    SDL_DestroyWindow(mWindow);
    SDL_Quit();
    TTF_Quit();
}

void Game::run()
{
    if (!mStates.empty())
    {
        mStates.back()->run();
    }
}

void Game::popState()
{
    if (!mStates.empty())
    {
        delete mStates.back();
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

   
