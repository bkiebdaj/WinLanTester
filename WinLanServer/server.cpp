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
  *  czyli po prostu wypelnia odpowiednie struktury
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

/** @brief recieve_seconds
  *
  * pobiera i zwraca liczbe sekund przez jaka serwer ma wysylac dane
  */
int Server::recieve_seconds()
{
    char buffer[512];
    recv(client_sock, buffer, 511, 0);
    int ile = atoi(buffer);
    std::cout<<atoi(buffer)<<" sekund(y/a)"<<std::endl;
    return ile;
}

/** @brief send_KB
  *
  * wysyla 4KB danych do klienta
  */
void Server::send_KB()
{
    char buffer[4096];
    send(client_sock, buffer, 4096, 0);
}

//watek, ktory przesyla dane do klienta dopoki zmienna "ile" nie jest zerem
DWORD WINAPI sending_KB(LPVOID param)
{
    void **table = (void**)param;
    Server *ser = (Server*)table[1];
    int *ile = (int*)table[0];
    while(*ile)
        ser->send_KB();
    std::cout<<"zakonczono przesylanie"<<std::endl;
    return 0;
}


//Procedura watku, ktory czeka na polaczenie z klientem
DWORD WINAPI waiting_for_client(LPVOID param)
{
    Server *ser = (Server*)param;
    ser->wating_for_connection();
    return 0;
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
        std::cout<<"polaczono!"<<std::endl; //gdy klient polaczy sie z serwerem
        int ile = recieve_seconds();        //serwer pobiera liczbe sekund, jak dlugo ma wysylac pakiety
        void *par1 = &ile;
        void *par2 = this;
        void **table = new void*[2];
        table[0] = par1;
        table[1] = par2;                    //po utworzeniu watku, ktory bedzie wysylal pakiety do klienta
        CreateThread(NULL, 0, sending_KB, (LPVOID)table, 0, NULL);
        Sleep(ile*1000);                    //usypiamy "glowny" watek na liczbe sekund przekazana przez klienta
        ile = 0;                            //nastepnie ustawiamy zmienna "ile" na "0" aby przerwac dzialanie
        closesocket(client_sock);           //wczesniej utworzonego watku i zamykamy polaczenie aby klient przestal pobierac dane
    }
}



/** @brief start
  *
  * odpalanie serwera ;>
  * jak juz to pisalem wyzej w tej metodzie tworzony jest watek,
  * ktory nasluchuje polaczenia z klientem,
  * jednoczesnie glowny watek czeka na wprowadzenie z klawiatury jakiejs liczby
  * po wpisaniu "0" serwer konczy dzialanie (tylko nie wpisujcie nic innego oprocz cyfr bo sie posypie:D)
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
            std::cout<<"zamykanie serwera"<<std::endl;
            system("PAUSE");
        }
    }
}

/** @brief clean
  *
  *  czyszczenie zasobow serwera
  *  wywolywac na koncu dzialania programu;>
  */
void Server::clean()
{
    closesocket(serv_sock);
    WSACleanup();
}

