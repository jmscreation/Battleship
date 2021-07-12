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
		gamecontroller = nullptr;
		
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

		if(mplay == nullptr) return false; // no multiplayer controller

		if(!mplay->update(elapsedTime)){
			std::cout << "Multiplayer system closed application\n";
			delete gamecontroller;
			gamecontroller = nullptr;

			delete mplay;
			mplay = nullptr;

			return false;
		}

		if(mplay->isConnected()){
			if(gamecontroller == nullptr){
				gamecontroller = new game::GameController(this, mplay);
			}
		} else {
			if(gamecontroller != nullptr) {
				delete gamecontroller;
				gamecontroller = nullptr;
				Clear(olc::BLANK);
			}
		}
		if(gamecontroller != nullptr){
			if(!gamecontroller->update(elapsedTime)){
				std::cout << "GameController closed application\n";
				mplay->clear(); // close mplay

				delete gamecontroller;
				gamecontroller = nullptr;

				delete mplay;
				mplay = nullptr;
				return false;
			}
		}
		
		return true;
	}
	
};


int main(int argc, char** argv){

	for(int i=1; i < argc; ++i){
		parameters.push_back(argv[i]);
	}

	Game app(800, 400, 1, 1);
	return 0;
}