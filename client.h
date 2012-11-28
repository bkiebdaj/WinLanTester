#ifndef CLIENT_H
#define CLIENT_H


class Client
{
    public:
        /** Default constructor */
        Client();
        /** Default destructor */
        virtual ~Client();
        /** Access data
         * \return The current value of data
         */
        WSADATA Getdata() { return data; }
        /** Set data
         * \param val New value to set
         */
        void Setdata(WSADATA val) { data = val; }
        /** Access sock
         * \return The current value of sock
         */
        SOCKET Getsock() { return sock; }
        /** Set sock
         * \param val New value to set
         */
        void Setsock(SOCKET val) { sock = val; }
        /** Access addr
         * \return The current value of addr
         */
        sockaddr_in Getaddr() { return addr; }
        /** Set addr
         * \param val New value to set
         */
        void Setaddr(sockaddr_in val) { addr = val; }
    protected:
    private:
        WSADATA data; //!< Member variable "data"
        SOCKET sock; //!< Member variable "sock"
        sockaddr_in addr; //!< Member variable "addr"
};

#endif // CLIENT_H
