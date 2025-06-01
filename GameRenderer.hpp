// GameRenderer.hpp
#ifndef GAMERENDERER_HPP
#define GAMERENDERER_HPP

#include "renderer.hpp"
#include "texture.hpp"
#include "piece.hpp"
#include "board.hpp"
#include "config.hpp"

class GameRenderer {
public:
    GameRenderer(Renderer* renderer);
    ~GameRenderer();

    void loadAssets();
    void render(const Board* board, const Piece& current, const Piece& next, const Piece& hold, bool showGhost, int score, int stage);

private:
    Renderer* renderer;

    Texture* tetrominoSprites;
    SDL_Rect tetrominoSpriteClips[7];

    Texture* playfieldFrame;
    SDL_Rect playfieldFrameClips[4];

    Texture* stageTextTexture;
    Texture* scoreTextTexture;

    void drawBoard(const Board* board);
    void drawPiece(const Piece& p, int offsetX, int offsetY);
    void drawCurrentPiece(const Piece& p);
    void drawNextPiece(const Piece& p);
    void drawHoldPiece(const Piece& p);
    void drawGhostPiece(const Piece& p, const Board* board);

    SDL_Color getStageColor(int stage);
};

#endif // GAMERENDERER_HPP
