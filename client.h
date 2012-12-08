#ifndef CLIENT_H
#define CLIENT_H
#include<winsock2.h>
#include<string>
class Client
{
    private:
        WSADATA data; //zmienna, która przechowuje informacje o Winsock'u (wersja itd.)
        SOCKET sock; //gniazdo dla naszego klienta
        sockaddr_in addr; //struktura przechowujaca
        std::string host_name; //nazwa serwera, z którym bedziemy sie laczyc

        bool init();
        u_long get_host(const std::string &h_name);

    public:
        /** Default constructor */
        Client();
        /** Default destructor */
        virtual ~Client();

        bool connect_to_host(const std::string &h_name, u_short port);
        bool send_seconds(const std::string &seconds);
        int recieve_KB();
        void close();
        void make_test(const std::string &seconds);
};

#endif // CLIENT_H
