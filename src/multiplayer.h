#pragma once

#include "SFML/Network.hpp"


class Multiplayer {
    sf::TcpListener server;
    sf::TcpSocket socket;

    const short port;
    bool ishost, isrunning, isconnected;

    sf::Packet in, out;

public:
    Multiplayer();
    virtual ~Multiplayer();

    void clear();
    void host(sf::IpAddress address = sf::IpAddress::Any);
    void connect(sf::IpAddress address);
    void update(float delta);

};