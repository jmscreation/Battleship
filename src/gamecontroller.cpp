#include "gamecontroller.h"

olc::PixelGameEngine* game::GameObj::pge = nullptr;
game::GameController* game::GameObj::ctrl = nullptr;

game::GameController::GameController(olc::PixelGameEngine* pge, Multiplayer* mp, int width, int height):
    pge(pge), mp(mp), width(width), height(height) {

    GameObj::pge = pge;
    GameObj::ctrl = this;

    screenW = pge->ScreenWidth();
    screenH = pge->ScreenHeight();

    cw = screenW / 2 / width;
    ch = screenH / height;
    hwidth = screenW / 2;

    endGame = false;
    delay = 0;

    selected = nullptr;
}

game::GameController::~GameController() {}

bool game::GameController::update(float delta) {
    control();
    render();

    return !endGame;
}

void game::GameController::control() {
    if(pge->GetKey(olc::ESCAPE).bHeld)
        endGame = true;

    if(delay > 0) delay--;

    for(auto& ship : ships) {
        ship->control();
    }
    for(auto& splash : splashes) {
        splash->control();
    }
    for(auto& hit : hits) {
        hit->control();
    }
}

void game::GameController::render() {
    pge->Clear(olc::BLACK);

    pge->DrawLine(hwidth,0, hwidth,screenH, 0xFF303030);

    for(int y=0; y<height; y++)
    for(int x=0; x<width; x++) {
        pge->Draw(x*cw, y*ch, 0xFF808080);
        pge->Draw(x*cw + hwidth, y*ch, 0xFF808080);
    }

    for(auto& ship : ships) {
        ship->draw();
    }
    for(auto& splash : splashes) {
        splash->draw();
    }
    for(auto& hit : hits) {
        hit->draw();
    }
}

void game::Ship::control() {
    if(ctrl->selected == this && ctrl->delay == 0) {
        if(pge->GetKey(olc::LEFT).bHeld) {

        }
        if(pge->GetKey(olc::RIGHT).bHeld) {

        }
        if(pge->GetKey(olc::UP).bHeld) {

        }
        if(pge->GetKey(olc::DOWN).bHeld) {
            
        }
    }
}
void game::Ship::draw() {
    for(int n=-1; n<=1; n++) {
        pge->FillRect(
            x * ctrl->cw + n * (dir == HOR) + 2,
            y * ctrl->ch + n * (dir == VERT) + 2,
            ctrl->cw - 4, ctrl->ch - 4,
            olc::WHITE
        );
    }
}

void game::Splash::draw() {
    pge->DrawCircle(x + ctrl->cw/2, y + ctrl->ch/2, t, olc::WHITE);
}