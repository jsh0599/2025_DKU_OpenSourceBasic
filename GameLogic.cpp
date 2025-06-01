// GameLogic.cpp
#include "GameLogic.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

GameLogic::GameLogic(Board* board)
    : board(board), holdFirstTime(true), holdUsed(false), ghostEnabled(true),
      score(0), stage(1), linesCleared(0), dropInterval(0.8f) {
    srand(static_cast<unsigned int>(time(nullptr)));
    nextPiece.piece_type = rand() % 7;
    nextPiece.rotation = 0;
    createNewPiece();
}

void GameLogic::reset() {
    score = 0;
    stage = 1;
    linesCleared = 0;
    dropInterval = 0.8f;
    holdFirstTime = true;
    holdUsed = false;
    nextPiece.piece_type = rand() % 7;
    nextPiece.rotation = 0;
    createNewPiece();
}

void GameLogic::handleAction(Action action) {
    switch (action) {
        case Action::move_down:
            currentPiece.r++;
            if (!board->isPositionLegal(currentPiece)) {
                currentPiece.r--;
                checkState();
            }
            break;
        case Action::move_left:
            currentPiece.c--;
            if (!board->isPositionLegal(currentPiece)) currentPiece.c++;
            break;
        case Action::move_right:
            currentPiece.c++;
            if (!board->isPositionLegal(currentPiece)) currentPiece.c--;
            break;
        case Action::drop:
            while (board->isPositionLegal(currentPiece)) currentPiece.r++;
            currentPiece.r--;
            checkState();
            break;
        case Action::rotate:
            currentPiece.rotation = (currentPiece.rotation + 1) % 4;
            if (!board->isPositionLegal(currentPiece))
                currentPiece.rotation = (currentPiece.rotation + 3) % 4;
            break;
        case Action::hold:
            if (holdFirstTime) {
                holdPiece = Piece(currentPiece);
                holdPiece.rotation = 0;
                createNewPiece();
                holdFirstTime = false;
                holdUsed = true;
            } else if (!holdUsed) {
                std::swap(currentPiece, holdPiece);
                holdPiece.rotation = 0;
                currentPiece.r = currentPiece.getInitialOffsetR();
                currentPiece.c = config::playfield_width / 2 + currentPiece.getInitialOffsetC();
                holdUsed = true;
            }
            break;
        default:
            break;
    }
}

void GameLogic::updateDropTimer(float delta) {
    // 구현 생략: 타이머 기반 자동 드롭
}

void GameLogic::step() {
    currentPiece.r++;
    if (!board->isPositionLegal(currentPiece)) {
        currentPiece.r--;
        checkState();
    }
}

bool GameLogic::isGameOver() const {
    return board->isGameOver();
}

const Piece& GameLogic::getCurrentPiece() const { return currentPiece; }
const Piece& GameLogic::getNextPiece() const { return nextPiece; }
const Piece& GameLogic::getHoldPiece() const { return holdPiece; }
bool GameLogic::isHoldUsed() const { return holdUsed; }
bool GameLogic::isFirstHold() const { return holdFirstTime; }
int GameLogic::getScore() const { return score; }
int GameLogic::getStage() const { return stage; }
float GameLogic::getDropInterval() const { return dropInterval; }
void GameLogic::setGhostEnabled(bool enabled) { ghostEnabled = enabled; }

void GameLogic::createNewPiece() {
    currentPiece.piece_type = nextPiece.piece_type;
    currentPiece.rotation = nextPiece.rotation;
    currentPiece.r = currentPiece.getInitialOffsetR();
    currentPiece.c = config::playfield_width / 2 + currentPiece.getInitialOffsetC();
    nextPiece.piece_type = rand() % 7;
    nextPiece.rotation = 0;
}

void GameLogic::checkState() {
    board->storePiece(currentPiece);
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
        createNewPiece();
    }
    holdUsed = false;
}
