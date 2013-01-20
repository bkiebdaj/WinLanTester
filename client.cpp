#include "client.h"
#include<iostream>
#include"Database.h"
#include"Data_test.h"
#include"Test.h"
Client::Client()
{
    ZeroMemory(&data, sizeof(data)); //funkcja, która zeruje strukturke
    sock = INVALID_SOCKET;
    ZeroMemory(&addr, sizeof(addr));
    host_name = "";
    started = false;
    start_thread = NULL;
}



/** @brief init
  *
  * funkcja inicjalizująca Winsock'a
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

/** @brief
  * @param h_name nazwa hosta moze byc przekazana jako adres IP np. "127.0.0.1" lub jako nazwa np. "localhost"
  * funkcja zwracajaca hosta jako liczbe calkowita
  */
u_long Client::get_host(const std::string &h_name)
{
    HOSTENT *host = gethostbyname(h_name.c_str()); //pobieranie hosta
    if(!host)                                      //jesli nazwa jest nieprawidlowa funkcja zwraca "NULL"
    {                                              //w takim wypadku sprawdzamy czy wpisany zostal adres IP serwera
        u_int address = inet_addr(h_name.c_str()); //"inet_addr" zamienia IP podany jako ciag znakow na liczbe calkowita
        host = gethostbyaddr((char *)address, 4, AF_INET);//pibieramy hosta po IP.... AF_INET == IPv4.. drugi parametr to dlugosc adresu w bajtach
        if(!host)           //jesli nie odnaleziono hosta to metoda zwraca NULL
        {
            std::cout<<"nie polaczono z hostem (get_host)!"<<std::endl;
            return 0;       //a cala metoda zwraca 0
        }
    }
    return *((u_long*)*host->h_addr_list);  //jesli wszystko pojdzie dobrze zwracamy adres hosta jako liczbe calkowita
}



/** @brief (one liner)
  *
  * polaczenie z serwerem
  */
bool Client::connect_to_host(const std::string &h_name, u_short port)
{
    if(init())      //inicjalizacja winsock'a
    {
        host_name = h_name;         //wypelnianie struktury sockaddr_in
        addr.sin_addr.s_addr = get_host(h_name);  //nazwa hosta
        addr.sin_family = AF_INET;      //IPv4
        addr.sin_port = htons(port);    //port hosta
        if(connect(sock, (sockaddr*)&addr, sizeof(sockaddr)) == SOCKET_ERROR )
        {                       //jesli funkcja "connect" wykona sie poprawnie zwraca "0"
            sock = INVALID_SOCKET;
            std::cout<<"nie udalo sie polaczyc z serwerem (connect)!"<<std::endl;
            return false;
        }
        return true;
    }
    return false;
}

/** @brief recieve_KB
  *
  * odbieranie pakietu od serwera
  * funkcja zwraca liczbe bajtow odebranych od serwera
  */
int Client::recieve_KB()
{
    char buffer[4096];
    int result = recv(sock,buffer, 4096, 0);
    return result;
}



/**@brief procedura watku, w ktorym klient odbiera dane od serwera
 *@param param do funkcji przekazujemy zmienna "param",
 *  ktora jest dynamiczna tablica typu "void" przechowuje wskazniki na klienta oraz na liczbe calkowita,
 *  w ktorej przechowujemy wynik
*/
DWORD WINAPI recieving_thread(LPVOID param)
{
    void **table = (void**)param;       //rzutowanie na podwojny wskaxnik typu void,
                                        //poniewaz pod taka postacia zostal przekazany parametr funkcji
    Client *ser = (Client*)table[0];    //pierwszy element tablicy to klient, stad rzutowanie na typ Client
    int *sum = (int*)table[1];          //drugi to liczba calkowita, ktora bedzie przechowywac sume bajtow odebranych od serwera
    while(ser->is_started()) //watek bedzie sie wykonywal dopoki nie zostanie uruchomiona metoda "stop_test"
    {
        *sum += ser->recieve_KB();      //metoda "recieve_KB" odbiera dane od serwera i zwraca ilosc bajtow odebranych
    }
    return 0;
}


/**@brief ten watek tworzy nowy test i co sekunde zapisuje do niego data_test
 *@param param parametrem jest obiekt klienta
*/
DWORD WINAPI starting_thread(LPVOID param)
{
    Client *client = (Client*)param; //rzutowanie na wskaźnik typu "Client"
    int sum = 0;
    void **parameters = new void*[2];
    parameters[0] = client;
    parameters[1] = &sum;
    Database baza;
             //laczymy sie z baza, tudziez tworzymy nowa jesli nie istnieje
    Test *newTest = baza.StworzTest();  //tworzymy nowy test w bazie
    actualTest = newTest ;
    int second = 1;                   //tworzy nastepny watek, ktory bedzie odbieral 4KB przeslanych danych
                                    //watek zakonczy sie kiedy wykonana zostanie metoda stop_test
    CreateThread(NULL, 0, recieving_thread, (LPVOID)parameters, 0, NULL);
    while(client->is_started())     //petla dziala dopoki nie zostanie wykonana metoda "stop_test"
    {                               //usypia watek na sekunde, aby po uplywie tego czasu
        Sleep(1000);                //dodac kolejny data_test do utworzonego testu
        baza.DodajKolejnyDataTest(newTest->GetID(), second, sum);   //dodajemy co sekunde nowy data_test do nowoutworzonego testu
        sum = 0;                    //zerujemy licznik przeslanych danych
        ++second;
        actualTest->AddDataTest(baza.ZwrocNowyDataTest());                //inkrementujemy zmienna, ktora przechowuje aktualna sekunde dzialania watku
    }
    baza.ZakonczTest(newTest->GetID());// metoda ZakonczTest ustawia nam date i czas konca testu podanego jako parametr
    closesocket(client->get_socket());//przerywa polaczenie z serwerem; dzieki temu serwer przestaje przesylac dane
    delete []parameters;
    return 0;
}

/** @brief start_test
  *
  * uruchamia watek testu
  */
void Client::start_test()
{
    if(!started)    //rozpoczynamy watek tylko jesli nie zostal jeszcze uruchomiony
    {               //takie zabezpieczenie, zeby nie uruchomic drugiego tego samego watku
        started = true;
        start_thread = CreateThread(NULL, 0, starting_thread,(LPVOID)this, 0, NULL);
    }
}

/** @brief stop_test
  *
  * konczy test
  */
void Client::stop_test()
{                               //ustawienie flagi "started" na "false" powoduje zakonczenie watku
    started = false;            //"starting_thread" jesli takowy jest uruchomiony
    WaitForSingleObject(start_thread,INFINITE); //czekamy na zakonczenie watku
}




/** @brief (one liner)
  *
  * zamykanie polaczenia i wyczyszczenie Winsock'a z pamieci
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
