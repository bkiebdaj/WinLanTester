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
        Test* StworzTest();
        void ZakonczTest(int id);
        Data_test* ZwrocNowyDataTest();
        Data_test* ZwrocDataTest(int id);
        vector<Test*> ZwrocWszystkieTesty();
        void DodajKolejnyDataTest(int id_testu, int tm, int wg);

};

#endif //DATABASE_H
