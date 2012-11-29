#include "server.h"
#include<iostream>
Server::Server()
{
    ZeroMemory(&data, sizeof(data));
    serv_sock = INVALID_SOCKET;
    ZeroMemory(&addr, sizeof(addr));
    client_sock = INVALID_SOCKET;
}

Server::~Server()
{
    //dtor
}


/** @brief init
  *
  * @todo: funkcja inicjalizujaca (praktycznie to samo co w kliencie)
  */
bool Server::init()
{
    int result = WSAStartup(MAKEWORD(2,2), &data);
    if(result)
    {
        std::cout<<"blad inicjalizacji (init)!"<<std::endl;
        return false;
    }
    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serv_sock == INVALID_SOCKET)
    {
        std::cout<<"blad socket'a"<<std::endl;
        return false;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(ADDR_ANY);

    return true;
}

/** @brief start
  *
  * odpalanie serwera ;>
  */
void Server::start(u_short port)
{
    if(init())
    {
        addr.sin_port = htons(port);
        if( bind(serv_sock, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR )
        {
            std::cout<<"wystartowal!"<<std::endl;
            listen(serv_sock, SOMAXCONN);
            while( client_sock = accept(serv_sock, NULL, NULL) )
            {
                std::cout<<"polaczono!"<<std::endl;
                closesocket(client_sock);
            }
        }
    }
}

void Server::stop()
{
    closesocket(serv_sock);
    WSACleanup();
}

