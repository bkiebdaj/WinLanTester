#ifndef SERVER_H
#define SERVER_H
#include<winsock2.h>

class Server
{
    private:
        WSADATA data; //!< Member variable "data"
        SOCKET serv_sock; //!< Member variable "serv_sock"
        sockaddr_in addr; //!< Member variable "addr"
        SOCKET client_sock; //!< Member variable "client_sock"
        bool init();


    public:
        /** Default constructor */
        Server();
        /** Default destructor */
        virtual ~Server();

        void start(u_short port);
        void wating_for_connection();
        int send_KB();
        void clean();
};

#endif // SERVER_H
