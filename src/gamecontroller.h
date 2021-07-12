#pragma once

#include "olcPixelGameEngine.h"
#include <vector>
#include <iostream>
#include <cmath>
#include "multiplayer.h"
#include "sfml/System.hpp"

namespace game {

    class GameObj;
    class GameController;
    class FPSLimiter;

    class FPSLimiter {
        float fps;
        sf::Clock timer;
        int lastTime, timeDiff;

    public:
        FPSLimiter(): fps(INFINITY), lastTime(-10000), timeDiff(0) {}
        FPSLimiter(float fps): fps(fps), lastTime(-10000), timeDiff(1000.f / fps) {}
        virtual ~FPSLimiter() {}
        
        void SetFPS(float fps) { this->fps = fps; timeDiff = int(1000.f / fps); }
        float GetFPS() { return fps; }
        void Wait() {
            int delta = timer.getElapsedTime().asMilliseconds() - lastTime;
            if(delta < timeDiff)
                sf::sleep(sf::milliseconds(timeDiff - delta));
            lastTime = timer.getElapsedTime().asMilliseconds();
        }
    };

    struct GameObj {
        static olc::PixelGameEngine* pge;
        static GameController* ctrl;

        enum Action {
            NONE, DESTROY, SELECT,
        };

        int x, y;
        virtual Action control() = 0;
        virtual void draw() = 0;
    };

    struct Ship: public GameObj {
        Ship(): dir(Dir::HOR), health(3), t(60) {}

        enum Dir {
            HOR, VERT,
        } dir;

        int health, t;

        virtual Action control();
        virtual void draw();
        bool collides();
    };
    struct Splash: public GameObj {
        Splash(bool local=false): t(0), local(local) {}
        
        int t;
        bool local;

        virtual Action control();
        virtual void draw();
    };
    struct Hit: public GameObj {
        Hit(bool local=false): t(0), local(local) {}

        int t;
        bool local;

        virtual Action control();
        virtual void draw();
    };

    struct MsgShoot {
        int x, y;
    };
    struct MsgShootReply {
        int x, y, status;
    };

    class GameController {
        static GameController* ctrl;
        olc::PixelGameEngine* pge;
        Multiplayer* mp;
        FPSLimiter fps;

    public:
        int width, height;
        int screenW, screenH;
        int cw, ch;
        int hwidth;

        bool endGame;
        int delay;
        
        Ship* selected;

        enum Commands {
            SHOOT,
            SHOOT_REPLY,
        };

        GameController(olc::PixelGameEngine* pge, Multiplayer* mp, int width=20, int height=20);
        virtual ~GameController();

        virtual bool update(float delta);

        static void receive(int cmd, const void* data, size_t len);

        void control();
        void render();

        void spawnShips(int num);
        int landHit(int x, int y);

        std::vector<Ship*> ships;
        std::vector<Splash*> splashes;
        std::vector<Hit*> hits;
    };
}