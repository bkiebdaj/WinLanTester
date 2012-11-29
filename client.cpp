#include "client.h"
#include<iostream>
Client::Client()
{
    ZeroMemory(&data, sizeof(data)); //funkcja, która zeruje strukturke
    sock = INVALID_SOCKET;
    ZeroMemory(&addr, sizeof(addr));
    host_name = "";
}



/** @brief init
  *
  * @todo: funkcja inicjalizująca Winsock'a
  */
bool Client::init()
{                                                    //WSAStartup inicjalizuje winsock'a
    int result = WSAStartup(MAKEWORD(2,2), &data);   //zwraca 0 jesli poprawnie odpalona
    if(result)                                       //MAKEWORD to makro, które tworzy//drugi parametr to referencja na strukturê WSADATA
    {
        std::cout<<"blad inicjalizacji winsock(init)!"<<std::endl;
        return false;
    }
                                                     //funkcja zwracajaca socketa
    sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  //pierwszy parametr okresla rodzine adresow z jakiej bedzie korzystal socket; AF_INET to IPv4
    if(sock == INVALID_SOCKET)                       //drugi parametr to typ socket'a; dla protokolu TCP uzywany jest typ SOCK_STREAM
    {                                                //trzeci parametr to protokol, z ktorego bedzie korzystal socket; IPPROTO_TCP to prot. TCP
        std::cout<<"blad socket'a (init)"<<std::endl;//funkcja zwraca INVALID_SOCKET jesli sie nie powiedzie
        return false;
    }
    return true;
}

/** @brief (one liner)
  *
  * funkcja zwracajaca hosta jako liczbe calkowita
  * pozniej jeszcze dopisze dokladnie o co chodzi:P
  */
u_long Client::get_host(const std::string &h_name)
{
    HOSTENT *host = gethostbyname(h_name.c_str());
    if(!host)
    {
        u_int address= inet_addr(h_name.c_str());
        host = gethostbyaddr((char *)address, 4, AF_INET);
        if(!host)
        {
            std::cout<<"nie polaczono z hostem (get_host)!"<<std::endl;
            return 0;
        }
    }
    return *((u_long*)*host->h_addr_list);
}






/** @brief (one liner)
  *
  * polaczenie z hostem
  */
bool Client::connect_to_host(const std::string &h_name, u_short port)
{
    if(init())
    {
        host_name = h_name;
        addr.sin_addr.S_un.S_addr = get_host(h_name);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if(connect(sock, (sockaddr*)&addr, sizeof(sockaddr)) == SOCKET_ERROR )
        {
            sock = INVALID_SOCKET;
            std::cout<<"blad gniazda"<<std::endl;
            std::cout<<"kod bledu = "<<WSAGetLastError()<<std::endl;
            return false;
        }
        return true;
    }
    return false;
}

/** @brief (one liner)
  *
  *zamykanie polaczenia
  */
void Client::close()
{
    closesocket(sock);
    WSACleanup();
}



Client::~Client()
{
    //dtor
}
