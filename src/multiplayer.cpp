#include "multiplayer.h"
#include <iostream>
#include <vector>

Multiplayer::Multiplayer(): socket(new sf::TcpSocket),
    tsender(new sf::Thread(sendHandle, this)), treceiver(new sf::Thread(receiveHandle, this)),
    useIP(sf::IpAddress::Any), port(55883), ishost(false), isrunning(false), isconnected(false),
    haveInData(false), haveOutData(false), syncStatus(sf::Socket::NotReady),
    onMessage([](int cmd, void* buf, size_t len) {
        std::cout << "unhandled message: [" << cmd << "] " << len << " bytes: ";

        for(int i=0; i < len; ++i){
            std::cout << ((char*)buf)[i];
            //std::cout << (int)((char*)buf)[i] << ",";
        }
        std::cout << "\n";

    }) {

    tsender->launch();
    treceiver->launch();
    
    socket->setBlocking(false);
    server.setBlocking(false);

    inbuffer = new char[1024 * 1024 * 2]; // allocate memory for packet data
}

Multiplayer::~Multiplayer() {
    tsender->terminate();
    treceiver->terminate();
    delete tsender;
    delete treceiver;
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
        sf::Packet ignore;

        if(ishost){
            status = server.accept(*socket);
        } else {
            status = socket->receive(ignore);
        }

        switch(status){
            case sf::Socket::Done:{
                std::cout << "Socket connected\n";
                isconnected = true;
                if(ishost){
                    server.close();
                    socket->send(ignore);
                }

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

    status = syncStatus;

    // Is Connected

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
        case sf::Socket::Done:{
            if(haveInData){
                sf::Int32 cmd;
                sf::Uint64 size;

                if(!(in >> cmd >> size)) break;

                const void* d = (const char*)in.getData() + sizeof(cmd) + sizeof(size);
                if(d != nullptr){
                    memcpy(inbuffer, d, size);
                }
                in.clear();

                onMessage(cmd, inbuffer, size);
                haveInData = false;
            }
            
            break;
        }
    }

    return true;
}

void Multiplayer::send(int command, const void* data, size_t length) {
    while(haveOutData);

    sf::Lock lock(mutex);

    out << sf::Int32(command) << sf::Uint64(length);
    out.append(data, length);
    haveOutData = true;
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

        if(me.haveOutData){
            sf::Lock lock(me.mutex);
            
            if(ready) {
                out.clear();
                out.append(me.out.getData(), me.out.getDataSize()); // copy packet
                me.out.clear();
                ready = false;
            }

            sf::Socket::Status status = me.socket->send(out);

            switch(status){
                case sf::Socket::Done:{
                    ready = true;
                    me.haveOutData = false;
                    break;
                }
                case sf::Socket::Partial:{
                    ready = false;
                    break;
                }
            }

            me.syncStatus = status;
        }
    }
}

void Multiplayer::receiveHandle(Multiplayer* _this) {
    
    Multiplayer& me = *_this;
    
    sf::Packet in;
    bool ready = false;

    while(1){
        sf::sleep(sf::milliseconds(2));
        
        if(!me.isconnected) continue;

        if(!me.haveInData){
            sf::Lock lock(me.mutex);

            sf::Socket::Status status = me.socket->receive(in);

            switch(status){
                case sf::Socket::Done:{
                    ready = true;
                    break;
                }
            }

            if(ready) {
                me.in.append(in.getData(), in.getDataSize());
                me.haveInData = true;
                ready = false;
            }

            me.syncStatus = status;
        }
    }
}