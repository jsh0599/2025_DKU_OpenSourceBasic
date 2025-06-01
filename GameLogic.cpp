// GameLogic.cpp
#include "GameLogic.hpp"
#include "game.hpp"
#include <iostream>

GameLogic::GameLogic(Board* board) : board(board), linesCleared(0), stage(1), score(0), dropInterval(0.8f), dropTimer(0.0f) {}

void GameLogic::createNewPiece(Piece& current, Piece& next) {
    current.piece_type = next.piece_type;
    current.rotation = next.rotation;
    current.r = current.getInitialOffsetR();
    current.c = config::playfield_width / 2 + current.getInitialOffsetC();

    for (int i = 0; i < 2; i++) {
        current.r++;
        if (!board->isPositionLegal(current)) {
            current.r--;
        }
    }
    if (current.piece_type > 1) {
        current.r++;
        if (!board->isPositionLegal(current)) {
            current.r--;
        }
    }

    next.piece_type = getRandom(0, 6);
    next.rotation = 0;
    next.r = config::next_box_y;
    next.c = config::next_box_x;
}

void GameLogic::checkState(Piece& current, Piece& next, Piece& hold, bool& holdUsed, bool& gameStarted) {
    board->storePiece(current);
    int cleared = board->clearFullLines();
    if (cleared > 0) {
        int points = 0;
        if (cleared == 1) points = 100;
        else if (cleared == 2) points = 300;
        else if (cleared == 3) points = 500;
        else if (cleared >= 4) points = 800;

        score += points;
    }

    linesCleared += cleared;
    while (linesCleared >= 1) {
        stage++;
        linesCleared--;
        dropInterval = std::max(0.1f, dropInterval - 0.1f);
        std::cout << "Stage Up! Now at Stage " << stage << ", drop interval = " << dropInterval << "s\n";
    }

    if (!board->isGameOver()) {
        createNewPiece(current, next);
    }
    holdUsed = false;
}

void GameLogic::movePieceDown(Piece& current, Piece& next, Piece& hold, bool& holdUsed, bool& gameStarted) {
    current.r++;
    if (!board->isPositionLegal(current)) {
        current.r--;
        checkState(current, next, hold, holdUsed, gameStarted);
    }
}

void GameLogic::handleEvent(Action action, Piece& current, Piece& next, Piece& hold, bool& holdFirst, bool& holdUsed, bool& gameStarted) {
    switch(action) {
        case Action::move_down:
            current.r++;
            if (!board->isPositionLegal(current)) {
                current.r--;
                checkState(current, next, hold, holdUsed, gameStarted);
            }
            break;
        case Action::move_left:
            current.c--;
            if (!board->isPositionLegal(current)) current.c++;
            break;
        case Action::move_right:
            current.c++;
            if (!board->isPositionLegal(current)) current.c--;
            break;
        case Action::drop:
            while (board->isPositionLegal(current)) current.r++;
            current.r--;
            checkState(current, next, hold, holdUsed, gameStarted);
            break;
        case Action::move_up:
        case Action::rotate:
            current.rotation = (current.rotation + 1) % 4;
            if (!board->isPositionLegal(current)) current.rotation = (current.rotation + 3) % 4;
            break;
        case Action::hold:
            if (holdFirst) {
                hold = Piece(current);
                hold.rotation = 0;
                createNewPiece(current, next);
                holdFirst = false;
                holdUsed = true;
            } else if (!holdUsed) {
                std::swap(current, hold);
                hold.rotation = 0;
                current.r = current.getInitialOffsetR();
                current.c = config::playfield_width / 2 + current.getInitialOffsetC();

                for (int i = 0; i < 2; i++) {
                    current.r++;
                    if (!board->isPositionLegal(current)) current.r--;
                }
                if (current.piece_type > 1) {
                    current.r++;
                    if (!board->isPositionLegal(current)) current.r--;
                }
                holdUsed = true;
            }
            break;
        case Action::pause:
            gameStarted = true;
            Game::getInstance()->pushPaused();
            break;
        default:
            break;
    }
}

bool GameLogic::isGameOver() const {
    return board->isGameOver();
}

int GameLogic::getScore() const {
    return score;
}

int GameLogic::getStage() const {
    return stage;
}

float GameLogic::getDropInterval() const {
    return dropInterval;
}

int GameLogic::getRandom(int lower_limit, int upper_limit) {
    return rand() % (upper_limit - lower_limit + 1) + lower_limit;
}

