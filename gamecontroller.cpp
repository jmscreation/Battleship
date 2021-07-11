#include "gamecontroller.h"


GameController::GameController(olc::PixelGameEngine& pge): pge(pge) {

}

void GameController::update(float delta) {
    for(Ship& ship : ships){
        drawShip(ship);
    }
}



void GameController::drawShip(GameController::Ship& ship) {
    olc::Pixel col = olc::WHITE;
    pge.Draw(ship.position, col);
    if(ship.direction == Ship::Vertical){
        pge.Draw(ship.position - olc::vi2d {0, 1}, col);
        pge.Draw(ship.position + olc::vi2d {0, 1}, col);
    }
    if(ship.direction == Ship::Horizontal){
        pge.Draw(ship.position - olc::vi2d {1, 0}, col);
        pge.Draw(ship.position + olc::vi2d {1, 0}, col);
    }
}