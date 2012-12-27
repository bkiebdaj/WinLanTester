#ifndef CLIENT_H
#define CLIENT_H
#include<winsock2.h>
#include<string>
class Client
{
    private:
        WSADATA data; //zmienna, kt�ra przechowuje informacje o Winsock'u (wersja itd.)
        SOCKET sock; //gniazdo dla naszego klienta
        sockaddr_in addr; //struktura przechowujaca
        std::string host_name; //nazwa serwera, z kt�rym bedziemy sie laczyc
        bool started;//flaga, ktora bedzie oznaczac czy test zostal wystartowany
        HANDLE start_thread; //uchwyt na watek, ktory uruchamia test

        bool init();
        u_long get_host(const std::string &h_name);

    public:
        /** Default constructor */
        Client();
        /** Default destructor */
        virtual ~Client();

        bool connect_to_host(const std::string &h_name, u_short port);
        int recieve_KB();
        void close();
        void start_test();
        void stop_test();
        const bool &is_started(){ return started; }
        const SOCKET &get_socket(){ return sock;}
};

#endif // CLIENT_H
