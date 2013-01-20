#ifndef DATABASE_H
#define DATABASE_H

#include <windows.h>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "Data_test.h"
#include "Test.h"

using namespace std;

class Database
{
    private:
        sqlite3 *database;
    public:
        Database();
        /**
        * @brief Tworzy nowy test
        *
        */
        Test* StworzTest();
        /**
        * @brief Konczy test dodajac aktualny czas zakonczenia
        *
        */
        void ZakonczTest(int id);
        /**
        * @brief Zwraca ostatnio dodany Data_test
        * @return Data_test*
        *
        */
        Data_test* ZwrocNowyDataTest();
        /**
        * @brief Zwraca Data_test o podanym id
        * @return Data_test*
        *
        */
        Data_test* ZwrocDataTest(int id);
        /**
        * @brief Zwraca wszystkie testy
        * @return vector<Test*>
        *
        */
        vector<Test*> ZwrocWszystkieTesty();
        /**
        * @brief Zwraca Test o podanym id wraz z nalezacymi do niego data_testami
        * @return Test*
        *
        */
        Test* ZwrocTest(int id);
        /**
        * @brief Dodaje kolejny Data_test
        * @param id_testu do ktorego dany data_test nalezy
        * @param tm czas data_testu
        * @param wg wartosc bedaca wynikiem testu
        *
        */
        Test* ZwrocTestPoDacie(string data);
        void DodajKolejnyDataTest(int id_testu, int tm, int wg);

};

#endif //DATABASE_H
