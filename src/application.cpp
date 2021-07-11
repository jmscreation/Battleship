#include "olcPixelGameEngine.h"

class Game : olc::PixelGameEngine {
public:
	Game(uint32_t w, uint32_t h, uint32_t px = 1, uint32_t py = 1) {
		sAppName = "Battleship In A Day";
		
		if(Construct(w, h, px, py)){
			Start();
		}
	}
public:
	bool OnUserCreate() {
		return true;
	}
	
	bool OnUserUpdate(float elapsedTime) {
		return true;
	}
	
};


int main(){
	Game app(400, 400);
	return 0;
}