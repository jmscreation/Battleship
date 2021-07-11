#include "gamecontroller.h"


game::GameController::GameController(olc::PixelGameEngine* pge, Multiplayer* mp, int width, int height):
    pge(pge), mp(mp), width(width), height(height) {

    GameObj::pge = &pge;
    GameObj::ctrl = this;

    screenW = pge.ScreenWidth();
    screenH = pge.ScreenHeight();

    cw = screenW / 2 / width;
    ch = screenH / height;
    hwidth = screenW / 2;

    selected = nullptr;
}

game::GameController::~GameController() {}

void game::GameController::update(float delta) {
    control();
    render();
}


void game::GameController::render() {
    pge->Clear(olc::BLACK);

    pge->DrawLine(hwidth,0, hwidth,screenH, 0xFF808080);

    for(int y=0; y<height; y++)
    for(int x=0; x<width; x++) {
        pge->Draw(x, y, olc::WHITE);
        pge->Draw(x+hwidth, y, olc::WHITE);
    }

    for(auto& ship : ships) {
        ship.draw();
    }
    for(auto& splash : splashes) {
        splash.draw();
    }
    for(auto& hit : hits) {
        hit.draw();
    }
}

void game::Ship::draw() {
    for(int n=-1; n<=1; n++) {
        pge->FillRect(
            x + n * (dir == HOR) + 2,
            y + n * (dir == VERT) + 2,
            ctrl->cw - 4, ctrl->ch - 4,
            olc::WHITE
        );
    }
}