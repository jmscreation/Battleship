#include "gamecontroller.h"

#define SHIPS_TO_SPAWN 10
#define DELAY_TIME 20

olc::PixelGameEngine* game::GameObj::pge = nullptr;
game::GameController* game::GameObj::ctrl = nullptr;

game::GameController::GameController(olc::PixelGameEngine* pge, Multiplayer* mp, int width, int height):
    pge(pge), mp(mp), fps(60), width(width), height(height) {

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

    spawnShips(SHIPS_TO_SPAWN);

    srand(time(nullptr));
}

game::GameController::~GameController() {
    for(auto& ship : ships)
        delete ship;
    for(auto& splash : splashes)
        delete splash;
    for(auto& hit : hits)
        delete hit;
}

bool game::GameController::update(float delta) {
    control();
    render();

    fps.Wait();

    return !endGame;
}

void game::GameController::control() {
    if(pge->GetKey(olc::ESCAPE).bHeld)
        endGame = true;

    if(delay > 0) delay--;

    bool select = false;

    for(int i=0; i<ships.size(); i++) {
        auto& ship = ships[i];
        switch(ship->control()){
            case GameObj::DESTROY: {
                if(selected == ship) selected = nullptr;
                delete ship;
                ships.erase(ships.begin() + i);
                i--;
                break;
            }
            case GameObj::SELECT: {
                selected = ship;
                select = true;
                break;
            }
        }
    }
    if(pge->GetMouse(olc::Mouse::LEFT).bPressed) {
        if(pge->GetMouseX() > hwidth) {
            if(!select) selected = nullptr;
        } else if(selected != nullptr) {
            int mx = pge->GetMouseX() / cw,
                my = pge->GetMouseY() / ch;
            
            if(rand() % 2) {
                Splash* sp = new Splash();
                sp->x = mx;
                sp->y = my;
                splashes.push_back(sp);
            } else {
                Hit* ht = new Hit();
                ht->x = mx;
                ht->y = my;
                hits.push_back(ht);
            }
        }
    }

    for(int i=0; i<splashes.size(); i++) {
        auto& splash = splashes[i];
        if(splash->control() == GameObj::DESTROY) {
            delete splash;
            splashes.erase(splashes.begin() + i);
            i--;
        }
    }
    for(int i=0; i<hits.size(); i++) {
        auto& hit = hits[i];
        if(hit->control() == GameObj::DESTROY) {
            delete hit;
            hits.erase(hits.begin() + i);
            i--;
        }
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

void game::GameController::spawnShips(int num) {
    for(int i=0; i<num; i++) {
        Ship* ship = new Ship();

        int tries = 10000;
        do {
            ship->dir = rand() % 2 ? Ship::HOR : Ship::VERT;
            ship->x = rand() % (width - 2) + 1;
            ship->y = rand() % (height - 2) + 1;
        } while(ship->collides() && --tries > 0);

        if(tries < 0) {
            delete ship;
            break;
        }

        ships.push_back(ship);
    }
}
bool game::GameController::landHit(int x, int y) {
    for(auto& ship : ships) {
        int dx = abs(x - ship->x),
            dy = abs(y - ship->y);
        
        if(ship->dir == Ship::HOR ? (dx < 2 && dy < 3) : (dy < 2 && dx < 3)) {
            ship->health--;
            Hit* hit = new Hit(true);
            hits.push_back(hit);
            return true;
        }
    }
    Splash* sp = new Splash(true);
    splashes.push_back(sp);
    return false;
}

game::GameObj::Action game::Ship::control() {
    if(health == 0) {
        if(--t) return DESTROY;
        return NONE;
    }

    if(ctrl->selected == this && ctrl->delay == 0) {
        bool L = pge->GetKey(olc::LEFT).bHeld,
             R = pge->GetKey(olc::RIGHT).bHeld,
             U = pge->GetKey(olc::UP).bHeld,
             D = pge->GetKey(olc::DOWN).bHeld;

        bool reset = true;

        do {
            if(dir == VERT) {
                if(L || R) {
                    dir = HOR;
                    if(collides()) dir = VERT; else break;
                }
            } else {
                if(U || D) {
                    dir = VERT;
                    if(collides()) dir = HOR; else break;
                }
            }

            if(L) {
                if(x > 1) {
                    x--;
                    if(collides()) x++; else break;
                }
            } else if(R) {
                if(x < ctrl->width-2) {
                    x++;
                    if(collides()) x--; else break;
                }
            } else if(U) {
                if(y > 1) {
                    y--;
                    if(collides()) y++; else break;
                }
            } else if(D) {
                if(y < ctrl->height-2) {
                    y++;
                    if(collides()) y--; else break;
                }
            }
        } while(reset = false);

        if(reset) {
            ctrl->delay = DELAY_TIME;
        }
    } else {
        if(pge->GetMouse(olc::Mouse::LEFT).bPressed) {
            int mx = abs((pge->GetMouseX() - ctrl->hwidth) / ctrl->cw - x),
                my = abs(pge->GetMouseY() / ctrl->ch - y);

            if(mx < (dir == HOR ? 2 : 1) && my < (dir == VERT ? 2 : 1)) {
                return SELECT;
            }
        }
    }
    return NONE;
}
void game::Ship::draw() {
    const static uint32_t colorTable[] = {
        0xFF000050, 0xFF0000A0,
        0xFF004080, 0xFF0080FF,
        0xFF008080, 0xFF00FFFF,
        0xFF808080, 0xFFFFFFFF,
    };
    
    for(int n=-1; n<=1; n++) {
        pge->FillRect(
            ctrl->hwidth + (x + n * (dir == HOR)) * ctrl->cw + 2,
            (y + n * (dir == VERT)) * ctrl->ch + 2,
            ctrl->cw - 4, ctrl->ch - 4,
            colorTable[health*2 + (ctrl->selected == this)]
        );
    }
}
bool game::Ship::collides() {
    for(auto& ship : ctrl->ships) {
        if(ship == this) continue;

        int dx = abs(ship->x - x),
            dy = abs(ship->y - y);

        if(ship->dir == dir) {
            if(dir == HOR ? (dy == 0 && dx < 3) : (dx == 0 && dy < 3))
                return true;
        } else {
            if(dx < 2 && dy < 2)
                return true;
        }
    }
    return false;
}

game::GameObj::Action game::Splash::control() {
    if(++t > ctrl->cw/2) return DESTROY;
    return NONE;
}
void game::Splash::draw() {
    pge->DrawCircle(
        x * ctrl->cw + ctrl->cw/2 + local * ctrl->hwidth,
        y * ctrl->ch + ctrl->ch/2,
        t,
        olc::WHITE
    );
}

game::GameObj::Action game::Hit::control() {
    if(++t > ctrl->cw/2) return DESTROY;
    return NONE;
}
void game::Hit::draw() {
    int cx = x * ctrl->cw + ctrl->cw/2 + local * ctrl->hwidth,
        cy = y * ctrl->ch + ctrl->ch/2,
        tt = t * 8 / 10;

    pge->DrawLine(cx-t, cy, cx+t, cy, 0xFF0000FF);
    pge->DrawLine(cx, cy-t, cx, cy+t, 0xFF0000FF);
    pge->DrawLine(cx-tt, cy-tt, cx+tt, cy+tt, 0xFF0000FF);
    pge->DrawLine(cx-tt, cy+tt, cx+tt, cy-tt, 0xFF0000FF);
}