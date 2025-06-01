// GameState.cpp (restructured)
#include "GameState.hpp"
#include "GameLogic.hpp"
#include "GameRenderer.hpp"
#include "game.hpp"

GameState::GameState(InputManager* manager)
    : State(manager),
      logic(nullptr),
      renderer(nullptr),
      board(nullptr),
      currentPiece(0, 0),
      nextPiece(0, 0),
      holdPiece(0, 0),
      ghostPiece(0, 0),
      hold_block_first_time(true),
      hold_block_used(false),
      game_just_started(true),
      time_snap1(0),
      time_snap2(0),
      nextStateID(STATE_NULL),
      isExited(false)
{
    board = new Board();
    logic = new GameLogic(board);
    renderer = new GameRenderer(board);

    nextPiece.piece_type = logic->getRandom(0, 6);
    nextPiece.rotation = 0;
    nextPiece.r = config::next_box_y;
    nextPiece.c = config::next_box_x;

    logic->createNewPiece(currentPiece, nextPiece);
}

GameState::~GameState() {
    if (!isExited) exit();
}

void GameState::initialize() {
    game_just_started = true;
    isExited = false;
    time_snap1 = SDL_GetTicks();
    time_snap2 = 0;
}

void GameState::exit() {
    if (isExited) return;
    isExited = true;

    delete logic;
    delete renderer;
    delete board;

    logic = nullptr;
    renderer = nullptr;
    board = nullptr;
}

void GameState::run() {
    switch (currentPhase) {
        case GAME_STARTED:
            if (game_just_started) {
                time_snap1 = SDL_GetTicks();
                game_just_started = false;
            }
            if (SDL_GetTicks() - time_snap1 < 3000) {
                while (mInputManager->pollAction()) {
                    if (mInputManager->isGameExiting()) {
                        nextStateID = STATE_EXIT;
                        return;
                    }
                    if (mInputManager->getAction() == Action::back) {
                        Game::getInstance()->popState();
                        return;
                    }
                }
                renderer->renderCountdown(3000 - (SDL_GetTicks() - time_snap1));
            } else {
                currentPhase = GAME_PLAYING;
                time_snap1 = SDL_GetTicks();
            }
            break;

        case GAME_PLAYING:
            if (mInputManager->isGameExiting()) {
                nextStateID = STATE_EXIT;
                return;
            }
            if (!logic->isGameOver()) {
                while (mInputManager->pollAction()) {
                    if (mInputManager->getAction() == Action::back) {
                        Game::getInstance()->popState();
                        return;
                    }
                    logic->handleEvent(mInputManager->getAction(), currentPiece, nextPiece, holdPiece, hold_block_first_time, hold_block_used, game_just_started);
                }
                time_snap2 = SDL_GetTicks();
                if (time_snap2 - time_snap1 >= static_cast<unsigned long long>(logic->getDropInterval() * 1000)) {
                    logic->movePieceDown(currentPiece, nextPiece, holdPiece, hold_block_used, game_just_started);
                    time_snap1 = SDL_GetTicks();
                }
                renderer->draw(currentPiece, nextPiece, holdPiece, ghostPiece, hold_block_first_time);
            } else {
                currentPhase = GAME_FINISHED;
            }
            break;

        case GAME_FINISHED:
            while (mInputManager->pollAction()) {
                if (mInputManager->getAction() == Action::back) {
                    Game::getInstance()->popState();
                    return;
                }
            }
            renderer->renderGameOver();
            break;
    }
}

void GameState::update() {
    // Not used in current design.
}
