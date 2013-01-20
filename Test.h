#ifndef TEST_H
#define TEST_H

#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include "Data_test.h"

using namespace std;



class Test
{
    private:
        int id;
        string start_time; // czas rozpoczecia test
        string finish_time; //czas zalonczenia calego testu
        vector<Data_test*> data_tests; // zawiera data_tests nalezace do danego testu
    public:
        Test();
        Test(int new_id, string start);
        Test(int new_id, string start, string finish);
        void SetID(int new_id);
        void SetStart_time(string start);
        void SetFinish_time(string finish);
        /**
        * @brief dodaje Data_test do danego testu
        */
        void AddDataTest(Data_test* dt);
        /**
        * @brief pobiera Data_test o podanum id
        * @return Data_test*
        *
        */
        Data_test* GetDataTest(int id);
        /**
        * @brief pobiera wszystekie Data_test z tego testu
        * @return vector<Data_test*>
        *
        */
        vector<Data_test*> GetAllDataTests();
        int GetID();
        string GetStart_time();
        string GetFinish_time();


};
extern Test *actualTest;
#endif //TEST_H
