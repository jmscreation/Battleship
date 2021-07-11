#pragma once

#include "olcPixelGameEngine.h"
#include <vector>
#include <list>
#include <iostream>



class GameController {
    olc::PixelGameEngine& pge;
public:

    struct Ship {
        olc::vi2d position;
        enum DIR {
            Horizontal, Vertical
        } direction;
    };

    std::list<Ship> ships;

    GameController(olc::PixelGameEngine& pge);
    virtual ~GameController();

    virtual void update(float delta);
    virtual void drawShip(Ship& ship);

};