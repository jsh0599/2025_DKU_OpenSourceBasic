// GameState.cpp
#include "GameState.hpp"
#include "game.hpp"
#include <SDL.h>
#include <cmath>

GameState::GameState(InputManager* manager)
    : State(manager), currentPhase(GAME_STARTED), board(nullptr),
      hold_block_first_time(true), hold_block_used(false),
      game_just_started(true), isExited(false),
      logic(nullptr), renderer(nullptr) {}

GameState::~GameState() {
    if (!isExited) {
        exit();
    }
}

void GameState::initialize() {
    currentPhase = GAME_STARTED;
    isExited = false;

    cleanup();

    board = new Board;
    logic = new GameLogic(board);
    renderer = new GameRenderer(board);

    srand(static_cast<unsigned>(time(0)));

    nextPiece.piece_type = logic->getRandom(0, 6);
    nextPiece.rotation = 0;
    nextPiece.r = config::next_box_y;
    nextPiece.c = config::next_box_x;

    logic->createNewPiece(currentPiece, nextPiece);
}

void GameState::exit() {
    if (isExited) return;
    isExited = true;
    cleanup();
}

void GameState::cleanup() {
    delete board; board = nullptr;
    delete logic; logic = nullptr;
    delete renderer; renderer = nullptr;
}

void GameState::run() {
    switch (currentPhase) {
        case GAME_STARTED: {
            if (game_just_started) {
                time_snap1 = SDL_GetTicks();
                game_just_started = false;
            }
            unsigned long long ms_passed = SDL_GetTicks() - time_snap1;
            if (ms_passed < 3000) {
                while (mInputManager->pollAction()) {
                    if (mInputManager->isGameExiting()) {
                        nextStateID = STATE_EXIT;
                        break;
                    }
                    if (mInputManager->getAction() == Action::back) {
                        Game::getInstance()->popState();
                        return;
                    }
                }
                Game::getInstance()->mRenderer->clearScreen();
                draw();
                int countdown_time = std::ceil((3000 - ms_passed) / 1000.0);
                renderer->drawCountdown(countdown_time);
                Game::getInstance()->mRenderer->updateScreen();
            } else {
                currentPhase = GAME_PLAYING;
                time_snap1 = SDL_GetTicks();
            }
            break;
        }
        case GAME_PLAYING: {
            if (mInputManager->isGameExiting()) {
                nextStateID = STATE_EXIT;
            } else if (!logic->isGameOver()) {
                bool shouldReturn = false;
                while (mInputManager->pollAction()) {
                    if (mInputManager->getAction() == Action::back) {
                        Game::getInstance()->popState();
                        shouldReturn = true;
                        break;
                    } else {
                        logic->handleEvent(mInputManager->getAction(), currentPiece, nextPiece, holdPiece,
                                           hold_block_first_time, hold_block_used, game_just_started);
                    }
                }
                if (shouldReturn) return;

                time_snap2 = SDL_GetTicks();
                if (time_snap2 - time_snap1 >= static_cast<unsigned long long>(logic->getDropInterval() * 1000)) {
                    logic->movePieceDown(currentPiece, nextPiece, holdPiece, hold_block_used, game_just_started);
                    time_snap1 = SDL_GetTicks();
                }
                Game::getInstance()->mRenderer->clearScreen();
                draw();
                Game::getInstance()->mRenderer->updateScreen();
            } else {
                currentPhase = GAME_FINISHED;
            }
            break;
        }
        case GAME_FINISHED: {
            if (!mInputManager->isGameExiting()) {
                while (mInputManager->pollAction() != 0) {
                    if (mInputManager->getAction() == Action::back) {
                        Game::getInstance()->popState();
                        return;
                    }
                }
                Game::getInstance()->mRenderer->clearScreen();
                draw();
                renderer->drawGameOver();
                Game::getInstance()->mRenderer->updateScreen();
            } else {
                nextStateID = STATE_EXIT;
            }
            break;
        }
    }
}

void GameState::update() {
    // No-op
}

void GameState::draw() {
    ghostPiece = currentPiece;
    while (board->isPositionLegal(ghostPiece)) ghostPiece.r++;
    ghostPiece.r--;

    renderer->drawAll(currentPiece, nextPiece, holdPiece, ghostPiece,
                      hold_block_first_time, logic->getStage(), logic->getScore());
}

