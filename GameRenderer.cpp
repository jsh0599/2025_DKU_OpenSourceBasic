// GameRenderer.cpp
#include "GameRenderer.hpp"
#include "game.hpp"
#include <algorithm>
#include <string>
#include <iostream>

const SDL_Color stageColors[] = {
    {249, 230, 207}, {100, 149, 237}, {186, 85, 211},
    {255, 160, 122}, {144, 238, 144}, {135, 206, 250},
    {255, 182, 193}, {255, 255, 224}, {119, 136, 153},
    {72, 61, 139}
};

GameRenderer::GameRenderer(Renderer* renderer) : renderer(renderer) {
    tetrominoSprites = new Texture();
    playfieldFrame = new Texture();
    stageTextTexture = new Texture();
    scoreTextTexture = new Texture();
}

GameRenderer::~GameRenderer() {
    delete tetrominoSprites;
    delete playfieldFrame;
    delete stageTextTexture;
    delete scoreTextTexture;
}

void GameRenderer::loadAssets() {
    tetrominoSprites->loadFromImage("tetrominoSprites.png");
    playfieldFrame->loadFromImage("playfieldFrame.png");

    for (int i = 0; i < 7; i++) {
        tetrominoSpriteClips[i] = {16 * i, 0, 16, 16};
    }
    for (int i = 0; i < 4; i++) {
        playfieldFrameClips[i] = {config::frame_sprite_size * i, 0, config::frame_sprite_size, config::frame_sprite_size};
    }
}

void GameRenderer::render(const Board* board, const Piece& current, const Piece& next, const Piece& hold, bool showGhost, int score, int stage) {
    SDL_Color bgColor = getStageColor(stage);
    renderer->setBackgroundColor(bgColor.r, bgColor.g, bgColor.b);
    renderer->clearScreen();

    drawBoard(board);
    drawCurrentPiece(current);
    if (showGhost && !board->isGameOver()) drawGhostPiece(current, board);
    if (hold.piece_type != -1) drawHoldPiece(hold);
    drawNextPiece(next);

    std::string stageText = "Stage: " + std::to_string(stage);
    stageTextTexture->loadFromText(stageText, Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    stageTextTexture->render(20, 20);

    std::string scoreText = "Score: " + std::to_string(score);
    scoreTextTexture->loadFromText(scoreText, Game::getInstance()->mRenderer->mediumFont, config::default_text_color);
    scoreTextTexture->render(20, 50);

    renderer->updateScreen();
}

void GameRenderer::drawBoard(const Board* board) {
    for (int i = 0; i < 2 * config::true_playfield_height; i++) {
        playfieldFrame->render(config::width_to_playfield - config::frame_sprite_size, config::height_to_playfield + i * config::frame_sprite_size, &playfieldFrameClips[0]);
        playfieldFrame->render(config::width_to_playfield + config::block_size * config::playfield_width - (config::frame_sprite_size - config::frame_width), config::height_to_playfield + i * config::frame_sprite_size, &playfieldFrameClips[0]);
    }
    playfieldFrame->render(config::width_to_playfield - config::frame_sprite_size, config::height_to_playfield + config::block_size * config::true_playfield_height - (config::frame_sprite_size - config::frame_width), &playfieldFrameClips[2]);
    playfieldFrame->render(config::width_to_playfield + config::block_size * config::playfield_width, config::height_to_playfield + config::block_size * config::true_playfield_height - (config::frame_sprite_size - config::frame_width), &playfieldFrameClips[3]);

    for (int i = 0; i < 2 * config::playfield_width; i++) {
        playfieldFrame->render(config::width_to_playfield + i * config::frame_sprite_size, config::height_to_playfield + config::block_size * config::true_playfield_height, &playfieldFrameClips[1]);
    }

    for (int row = 0; row < config::playfield_height; row++) {
        for (int col = 0; col < config::playfield_width; col++) {
            if (!board->isBlockFree(row, col)) {
                tetrominoSprites->render(config::width_to_playfield + col * config::block_size, config::height_to_playfield + (row - (config::playfield_height - config::true_playfield_height)) * config::block_size, &tetrominoSpriteClips[board->getTetromino(row, col)]);
            }
        }
    }
}

void GameRenderer::drawPiece(const Piece& p, int offsetX, int offsetY) {
    for (int row = 0; row < config::matrix_blocks; row++) {
        for (int col = 0; col < config::matrix_blocks; col++) {
            if (p.getBlockType(row, col) != 0) {
                tetrominoSprites->render(offsetX + col * config::block_size, offsetY + row * config::block_size, &tetrominoSpriteClips[p.piece_type]);
            }
        }
    }
}

void GameRenderer::drawCurrentPiece(const Piece& p) {
    drawPiece(p, config::width_to_playfield + p.c * config::block_size, config::height_to_playfield + (p.r - (config::playfield_height - config::true_playfield_height)) * config::block_size);
}

void GameRenderer::drawNextPiece(const Piece& p) {
    drawPiece(p, config::next_box_x, config::next_box_y);
}

void GameRenderer::drawHoldPiece(const Piece& p) {
    drawPiece(p, config::hold_box_x, config::hold_box_y);
}

void GameRenderer::drawGhostPiece(const Piece& p, const Board* board) {
    Piece ghost = p;
    while (board->isPositionLegal(ghost)) ghost.r++;
    ghost.r--;

    tetrominoSprites->setAlphaMode(config::transparency_alpha);
    drawCurrentPiece(ghost);
    tetrominoSprites->setAlphaMode(255);
}

SDL_Color GameRenderer::getStageColor(int stage) {
    int index = std::min(stage - 1, static_cast<int>(sizeof(stageColors) / sizeof(stageColors[0])) - 1);
    return stageColors[index];
}
