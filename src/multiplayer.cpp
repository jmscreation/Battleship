#include "multiplayer.h"
#include <iostream>
#include <vector>


Multiplayer::Multiplayer(): port(55883), ishost(false), isrunning(false), isconnected(false) {
    socket.setBlocking(false);
    server.setBlocking(false);
}

Multiplayer::~Multiplayer() {

}

void Multiplayer::clear() {
    socket.disconnect();
    server.close();

    isconnected = false;
    ishost = false;
    isrunning = false;
}

void Multiplayer::host(sf::IpAddress address) {
    if(isrunning) return;

    clear();
    server.listen(port, address);
    ishost = true;
    isrunning = true;
}

void Multiplayer::connect(sf::IpAddress address) {
    if(isrunning) return;

    clear();
    socket.connect(address, port, sf::seconds(10.0f));
    ishost = false;
    isrunning = true;
}

void Multiplayer::update(float delta){
    if(!isrunning) return;
    
    sf::Socket::Status status;

    if(!isconnected){

        if(ishost){
            status = server.accept(socket);
        } else {
            out.clear();
            out << sf::String("connect");
            status = socket.send(out);
        }

        switch(status){
            case sf::Socket::Done:{
                std::cout << "Client connected\n";
                isconnected = true;
                break;
            }
            case sf::Socket::Error:{
                std::cout << "Server error\n";
                clear();
                break;
            }
            case sf::Socket::NotReady:{
                break;
            }
            
        }

        return;
    }


    // Is Connected

    status = socket.send(out);
    out.clear();

    socket.receive(in);

}