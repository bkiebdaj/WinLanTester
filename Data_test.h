#ifndef DATA_TEST_H
#define DATA_TEST_H


#include <windows.h>
#include <string>
#include <iostream>

using namespace std;

class Data_test
{
    private:
        int id;
        int test_id;
        int time;
        int weight;
    public:
        Data_test();
        Data_test(int new_id, int new_test_id, int new_time, int new_weight);
        void SetID(int new_id);
        void SetTestID(int new_id);
        void SetTime(int tm);
        void SetWeight(int wg);
        int GetID();
        int GetTestID();
        int GetTime();
        int GetWeight();


};



#endif //DATA_TEST
