// GameLogic.hpp
#ifndef GAMELOGIC_HPP
#define GAMELOGIC_HPP

#include "board.hpp"
#include "piece.hpp"
#include "config.hpp"
#include "inputmanager.hpp"

class GameLogic {
public:
    GameLogic(Board* board);

    void reset();
    void handleAction(Action action);
    void updateDropTimer(float delta);
    void step();
    bool isGameOver() const;

    const Piece& getCurrentPiece() const;
    const Piece& getNextPiece() const;
    const Piece& getHoldPiece() const;
    bool isHoldUsed() const;
    bool isFirstHold() const;

    int getScore() const;
    int getStage() const;
    float getDropInterval() const;

    void setGhostEnabled(bool enabled);

private:
    void createNewPiece();
    void checkState();

    Board* board;
    Piece currentPiece {0, 0};
    Piece nextPiece {0, 0};
    Piece holdPiece {0, 0};

    bool holdFirstTime;
    bool holdUsed;
    bool ghostEnabled;

    int score;
    int stage;
    int linesCleared;
    float dropInterval;
};

#endif // GAMELOGIC_HPP
