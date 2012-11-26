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
        string start_time;
        string finish_time;
        vector<Data_test*> data_tests;
    public:
        Test();
        Test(int new_id, string start);
        Test(int new_id, string start, string finish);
        void SetID(int new_id);
        void SetStart_time(string start);
        void SetFinish_time(string finish);
        void AddDataTest(Data_test* dt);
        Data_test* GetDataTest(int id);
        vector<Data_test*> GetAllDataTests();
        int GetID();
        string GetStart_time();
        string GetFinish_time();


};

#endif //TEST_H
