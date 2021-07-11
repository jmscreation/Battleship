#include "olcPixelGameEngine.h"
#include "gamecontroller.h"
#include "multiplayer.h"

#include <vector>
#include <string>

std::vector<std::string> parameters;

bool findParameter(const std::string& para, std::string& value){
	auto f = std::find(parameters.begin(), parameters.end(), para);
	bool found = (f != parameters.end());

	if(found && (f + 1) != parameters.end()) value = *(f + 1);

	return found;
}

class Game : olc::PixelGameEngine {
public:
	Game(uint32_t w, uint32_t h, uint32_t px = 1, uint32_t py = 1) {
		sAppName = "Battleship In A Day";
		
		if(Construct(w, h, px, py)){
			Start();
		}
	}
	game::GameController* gamecontroller;
	Multiplayer* mplay;

public:
	bool OnUserCreate() {
		mplay = new Multiplayer;
		gamecontroller = new game::GameController(this, mplay);
		
		std::string ip = "";
		if(findParameter("connect", ip)){
			mplay->connect(sf::IpAddress(ip));
		}
		if(findParameter("host", ip)){
			if(ip != "")
				mplay->host(sf::IpAddress(ip));
			else
				mplay->host();
		}

		return true;
	}
	
	bool OnUserUpdate(float elapsedTime) {

		mplay->update(elapsedTime);
		gamecontroller->update(elapsedTime);

		return true;
	}
	
};


int main(int argc, char** argv){

	for(int i=1; i < argc; ++i){
		parameters.push_back(argv[i]);
	}

	Game app(400, 400);
	return 0;
}