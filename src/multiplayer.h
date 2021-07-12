#pragma once

#include "SFML/Network.hpp"
#include "message.h"

#include <atomic>
#include <functional>


class Multiplayer {
public:
    typedef std::function<void (int, void*, size_t)> CallbackFunction;

private:
    sf::TcpListener server;
    sf::TcpSocket* socket;
    
    sf::Thread *tsender, *treceiver;
    sf::Mutex mutex;

    sf::IpAddress useIP;
    const short port;

    std::atomic<bool> ishost, isrunning, isconnected;
    std::atomic<sf::Socket::Status> syncStatus;

    sf::Packet in, out;

    sf::Clock connectTimeout;

    CallbackFunction onMessage;

    char* inbuffer;


public:
    Multiplayer();
    virtual ~Multiplayer();

    static void sendHandle(Multiplayer* ths);
    static void receiveHandle(Multiplayer* ths);

    inline bool isHost() { return ishost; }
    inline bool isConnected() { return isconnected; }

    void clear();
    void host(sf::IpAddress address = sf::IpAddress::Any);
    void connect(sf::IpAddress address);
    bool update(float delta);

    void send(int command, const void* data, size_t length);
    void updateCallback(CallbackFunction cb);

};