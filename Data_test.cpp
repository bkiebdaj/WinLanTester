#include "Data_test.h"

Data_test::Data_test()
{
    id = 0;
    test_id = 0;
    time = 0;
    weight = 0;
}

Data_test::Data_test(int new_id, int new_test_id, int new_time, int new_weight)
{
    id = new_id;
    test_id = new_test_id;
    time = new_time;
    weight = new_weight;
}
void Data_test::SetID(int new_id)
{
    id = new_id;
}
void Data_test::SetTestID(int new_id)
{
    test_id = new_id;
}
void Data_test::SetTime(int tm)
{
    time = tm;
}
void Data_test::SetWeight(int wg)
{
    weight = wg;
}
int Data_test::GetID()
{
    return id;
}
int Data_test::GetTestID()
{
    return test_id;
}
int Data_test::GetTime()
{
    return time;
}
int Data_test::GetWeight()
{
    return weight;
}
