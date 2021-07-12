#include "multiplayer.h"
#include <iostream>
#include <vector>

Multiplayer::Multiplayer(): socket(new sf::TcpSocket), tsender(new sf::Thread(sendHandle, this)),
    useIP(sf::IpAddress::Any), port(55883), ishost(false), isrunning(false), isconnected(false),
    onMessage([](int cmd, void* buf, size_t len) { std::cout << "unhandled message: [" << cmd << "] " << len << " bytes\n";}) {

    tsender->launch();
    
    socket->setBlocking(false);
    server.setBlocking(false);

    inbuffer = new char[1024 * 1024 * 2]; // allocate memory for packet data
}

Multiplayer::~Multiplayer() {
    tsender->terminate();
    delete tsender;
    delete socket;
    delete[] inbuffer;
}

void Multiplayer::clear() {
    server.close();

    socket->disconnect();
    delete socket;

    socket = new sf::TcpSocket;
    socket->setBlocking(false);

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
    std::cout << "Connecting to " << address.toString() << "...\n";

    clear();
    socket->connect(address, port);
    connectTimeout.restart();

    ishost = false;
    isrunning = true;
    useIP = address;
}

bool Multiplayer::update(float delta){
    if(!isrunning) return true;
    
    sf::Socket::Status status;

    if(!isconnected){

        if(ishost){
            status = server.accept(*socket);
        } else {
            status = socket->receive(in);
        }

        switch(status){
            case sf::Socket::Done:{
                std::cout << "Client connected\n";
                isconnected = true;
                server.close();

                break;
            }
            case sf::Socket::Disconnected:
            case sf::Socket::NotReady:{
                if(!ishost){
                    if(connectTimeout.getElapsedTime().asSeconds() > 10.0f){
                        clear();
                        if(showQuestion("Failed to connect to remote server. Try Again?", "Error Connecting", MSG_ERROR)) {
                            connect(useIP);
                        } else {
                            return false;
                        }
                    }
                }
                break;
            }
            case sf::Socket::Error:{
                std::cout << "Server error\n";
                bool reconnect = !ishost;
                clear();
                if(reconnect) connect(useIP);
                break;
            }
            
        }

        return true;
    }


    // Is Connected

    do {
        status = socket->receive(in);

        switch(status) {
            case sf::Socket::Disconnected:
            case sf::Socket::Error:{
                std::cout << "Socket error\n";
                
                showMessage(ishost ? "Error: Client Disconnected!" : "Error: Server Disconnected!", "Error", MSG_ERROR);
                bool _ishost = ishost;
                clear();
                
                if(_ishost){
                    host(useIP);
                 } else {
                    connect(useIP);
                 }
                break;
            }
            case sf::Socket::Partial:{
                std::cout << "Receiving partial packet\n";
                sf::sleep(sf::milliseconds(2));
                continue;
            }
            case sf::Socket::Done:{
                do {
                    sf::Int32 cmd;
                    sf::Uint64 size;

                    if(!(in >> cmd >> size)) break;

                    const void* d = in.getData();
                    if(d != nullptr){
                        memcpy(inbuffer, d, size);
                    }
                    in.clear();

                    onMessage(cmd, inbuffer, size);
                } while(!in.endOfPacket());

                break;
            }
        }

        break; // do not loop
    } while(1);

    return true;
}

void Multiplayer::send(int command, const void* data, size_t length) {
    sf::Lock lock(mutex);

    out << sf::Int32(command) << sf::Uint64(length);
    out.append(data, length);
}

void Multiplayer::updateCallback(Multiplayer::CallbackFunction cb) {
    onMessage = cb;
}

void Multiplayer::sendHandle(Multiplayer* _this){
    Multiplayer& me = *_this;
    
    sf::Packet out;
    bool ready = true;

    while(1){
        sf::sleep(sf::milliseconds(2));
        
        if(!me.isconnected) continue;

        if(ready) {
            sf::Lock lock(me.mutex);
            out.clear();
            out.append(me.out.getData(), me.out.getDataSize()); // copy packet
            me.out.clear();
            ready = false;
        }

        sf::Socket::Status status = me.socket->send(out);

        switch(status){
            case sf::Socket::Done:{
                ready = true;
                break;
            }
            case sf::Socket::Partial:{
                ready = false;
                break;
            }
        }
    }
}