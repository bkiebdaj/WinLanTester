#include "server.h"
#include<iostream>
#include<windows.h>
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
  *  funkcja inicjalizujaca (praktycznie to samo co w kliencie)
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
        std::cout<<"blad socket'a (init)!"<<std::endl;
        return false;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(ADDR_ANY);

    return true;
}

//Procedura watku, ktory czeka na polaczenie z klientem
DWORD WINAPI waiting_for_client(LPVOID param)
{
    Server *ser = (Server*)param;
    ser->wating_for_connection();
}

/** @brief wating_for_connection
  *
  * funkcja wywolywana w watku powyzej aby mozna bylo
  *  zakonczyc dzialanie serwera wpisujac "0" w konsoli
  * funkcja wywoluje metode "accept", ktora czeka na polaczenie z klientem
  * zwraca SOCKET do klienta
  * narazie wypisuje tylko, ze polaczenie zostalo nawiazane i od razu zamyka polaczenie
  */
void Server::wating_for_connection()
{
    while( client_sock = accept(serv_sock, NULL, NULL) )
    {
        std::cout<<"polaczono!"<<std::endl;
        closesocket(client_sock);
    }
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
            listen(serv_sock, SOMAXCONN);   //ustawienie socketa servera na nasluchiwanie polaczenia klienta
            HANDLE thread;
            int chodzi = 1;             //tworzenie watku, ktory bedzie czekal na polaczenie z klientem
            thread = CreateThread(NULL, 0, waiting_for_client, (LPVOID)this, 0, NULL);
            while(chodzi)               //glowny watek czeka na wprowadzenie jakiejs liczby z konsoli
            {                           //jesli bedzie to "0" funkcja zabije watek, ktory czeka na polaczenie
                std::cin>>chodzi;       //z klientem i konczy swoje dzialanie
            }
            TerminateThread(thread,-1);
        }
    }
}

void Server::clean()
{
    closesocket(serv_sock);
    WSACleanup();
}

