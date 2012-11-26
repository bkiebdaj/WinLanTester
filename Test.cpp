#include "Test.h"

Test::Test()
{
    id = 0;
    start_time = "";
    finish_time = "";
}
Test::Test(int new_id, string start)
{
    id = new_id;
    start_time = start;
    finish_time = "";
}
Test::Test(int new_id, string start, string finish)
{
    id = new_id;
    start_time = start;
    finish_time = finish;
}
void Test::SetID(int new_id)
{
    id = new_id;
}
void Test::SetStart_time(string start)
{
    start_time = start;
}
void Test::SetFinish_time(string finish)
{
    finish_time = finish;
}
int Test::GetID()
{
    return id;
}
string Test::GetStart_time()
{
    return start_time;
}
string Test::GetFinish_time()
{
    return finish_time;
}
void Test::AddDataTest(Data_test* dt)
{
    data_tests.push_back(dt);
}
Data_test* Test::GetDataTest(int id)
{
    Data_test* tmp = new Data_test();
    for(int i = 0; i < data_tests.size();i++)
    {
        if(data_tests[i]->GetID() == id)
        {
            return data_tests[i];
        }
    }
    cout << " Niema data_test o podanym id";
    return tmp;
}
vector<Data_test*> Test::GetAllDataTests()
{
    return data_tests;
}
