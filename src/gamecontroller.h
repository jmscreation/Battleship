#pragma once

#include "olcPixelGameEngine.h"
#include <vector>
#include <iostream>
#include "multiplayer.h"

namespace game {

    class GameController;

    struct GameObj {
        static olc::PixelGameEngine* pge;
        static GameController* ctrl;

        int x, y;
        virtual void control() = 0;
        virtual void draw() = 0;
    };

    struct Ship: public GameObj {
        enum DIR {
            HOR, VERT,
        } dir;

        int health;

        virtual void control();
        virtual void draw();
    };
    struct Splash: public GameObj {
        int t;

        virtual void control();
        virtual void draw();
    };
    struct Hit: public GameObj {
        int t;

        virtual void control();
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

        bool endGame;
        int delay;
        
        Ship* selected;

        GameController(olc::PixelGameEngine* pge, Multiplayer* mp, int width=20, int height=20);
        virtual ~GameController();

        virtual bool update(float delta);

        void control();
        void render();

        std::vector<Ship*> ships;
        std::vector<Splash*> splashes;
        std::vector<Hit*> hits;
    };

}