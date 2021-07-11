#pragma once

#include "olcPixelGameEngine.h"
#include <vector>
#include <list>
#include <iostream>
#include "multiplayer.h"

namespace game {

    class GameController;

    class GameObj {
    public:
        static olc::PixelGameEngine* pge;
        static GameController* ctrl;

        olc::vi2d pos;
        virtual void draw() = 0;
    };

    class Ship: public GameObj {
        enum DIR {
            HOR, VERT,
        } dir;

        int health;

        virtual void draw();
    };
    class Splash: public GameObj {
        int t;

        virtual void draw();
    };
    class Hit: public GameObj {
        int t;

        virtual void draw();
    };

    class GameController {
        olc::PixelGameEngine* pge;
        Multiplayer* mp;

    public:
        int width, height;
        int screenW, screenH;
        int cw, ch;
        int hwidth;
        
        Ship* selected;

        GameController(olc::PixelGameEngine* pge, Multiplayer* mp, int width=20, int height=20);
        virtual ~GameController();

        virtual void update(float delta);

        void control();
        void render();

        std::vector<Ship> ships;
        std::vector<Splash> splashes;
        std::vector<Hit> hits;
    };

}