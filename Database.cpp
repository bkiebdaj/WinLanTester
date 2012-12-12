#include "Database.h"

using namespace std;

Database::Database()
{

    sqlite3_open("Database.sqlite", &database);

    sqlite3_stmt *statement;
    /* pozwolilem sobie zakomentowac do testow ;)......Igor
    // na razie usuwam tabele za kazdym razem w celach testowych
    if(sqlite3_prepare_v2(database, "DROP TABLE test", -1, &statement, 0) == SQLITE_OK) // metoda sqlite3_prepare_v2 kompiluje SQL querry do byte-code
	{
		int result = 0;
        result = sqlite3_step(statement);  // wykonujemy nastepny krok polecenia sql, w tym przypadku z reszta mamy tylko jeden krok :). Po SELECT uzywamy tej metody do przechodzenia po kolejnych wierszach
		sqlite3_finalize(statement); // usuwamy skompilowany statement
	}
    if(sqlite3_prepare_v2(database, "DROP TABLE data_test", -1, &statement, 0) == SQLITE_OK)
	{
		int result = 0;
        result = sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

    if(sqlite3_prepare_v2(database, "PRAGMA foreign_keys = ON;", -1, &statement, 0) == SQLITE_OK)
	{
		int result = 0;
        result = sqlite3_step(statement);
		sqlite3_finalize(statement);
	}
    */

	string zapytanie =  (string)"CREATE TABLE IF NOT EXISTS test ("
                                    +"id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                    +"start_date TEXT,"     // Date zapisujemy jako string bo niema typu datowego w sqlite. Format "YYYY-MM-DD HH:MM:SS".
                                    +"finish_date TEXT);";

	if(sqlite3_prepare_v2(database, zapytanie.c_str(),-1, &statement, 0) == SQLITE_OK)
	{
		int result = 0;
        result = sqlite3_step(statement);
		sqlite3_finalize(statement);
	}
	string zapytanie2 = (string)"CREATE TABLE IF NOT EXISTS data_test ("
                                    +"id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                    +"test_id INTEGER,"
                                    +"time INTEGER,"
                                    +"weight INTEGER, "
                                    +"FOREIGN KEY(test_id) REFERENCES test(id));";
	if(sqlite3_prepare_v2(database, zapytanie2.c_str(),-1, &statement, 0) == SQLITE_OK)
	{
		int result = 0;
        result = sqlite3_step(statement);
		sqlite3_finalize(statement);
	}
	/*
    if(sqlite3_prepare_v2(database, "INSERT INTO test(start_date, finish_date) VALUES('2012-11-23 19:19:00', '2012-11-23 19:20:00');", -1, &statement, 0) == SQLITE_OK)
	{
		int result = 0;
        result = sqlite3_step(statement);
		sqlite3_finalize(statement);
	}
    if(sqlite3_prepare_v2(database, "INSERT INTO data_test(test_id, time, weight) VALUES(1,22,33);", -1, &statement, 0) == SQLITE_OK)
	{
		int result = 0;
        result = sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

    if(sqlite3_prepare_v2(database, "INSERT INTO data_test(test_id, time, weight) VALUES(1,22,40);", -1, &statement, 0) == SQLITE_OK)
	{
		int result = 0;
        result = sqlite3_step(statement);
		sqlite3_finalize(statement);
	}*/
}
Test* Database::StworzTest()
{
    sqlite3_stmt *statement;

    if(sqlite3_prepare_v2(database, "INSERT INTO test(start_date,finish_date) VALUES(strftime('%Y-%m-%d %H:%M:%S','now'),'brak');", -1, &statement, 0) == SQLITE_OK)
	{
		int result = 0;
        result = sqlite3_step(statement);
		sqlite3_finalize(statement);
	}
	else
	{
	    cout << "Blad w insercie" <<endl;
	}

    Test *tmp = new Test();
    if(sqlite3_prepare_v2(database, "SELECT id, start_date FROM test WHERE id = (SELECT max(id) FROM test);", -1, &statement, 0) == SQLITE_OK)
	{
		int result = 0;
        result = sqlite3_step(statement);

        if(result == SQLITE_ROW)
        {
            string s = (char*)sqlite3_column_text(statement, 0);
            int i;
            istringstream iss(s);
            iss >> i;
            tmp->SetID(i);

            s = (char*)sqlite3_column_text(statement, 1);
            tmp->SetStart_time(s);

        }

		sqlite3_finalize(statement);
	}
    return tmp;
}
void Database::ZakonczTest(int id)
{
    ostringstream ss;
    ss << id;
    string param = ss.str();
    sqlite3_stmt *statement;
    string querry = (string)"UPDATE test SET finish_date = strftime('%Y-%m-%d %H:%M:%S','now') WHERE id="+param+";";

    if(sqlite3_prepare_v2(database, querry.c_str(), -1, &statement, 0) == SQLITE_OK)
	{
		int result = 0;
        result = sqlite3_step(statement);
		sqlite3_finalize(statement);
	}
}
Data_test* Database::ZwrocNowyDataTest()
{
    sqlite3_stmt *statement;
    Data_test* dtest = new Data_test();
    if(sqlite3_prepare_v2(database, "SELECT * FROM data_test WHERE id = (SELECT max(id) FROM data_test);", -1, &statement, 0) == SQLITE_OK)
	{
        int cols = sqlite3_column_count(statement);
		int result = 0;
		vector<int> data;
		int tmp;
		stringstream iss;
		while(true)
		{
			result = sqlite3_step(statement);

			if(result == SQLITE_ROW)
			{
				for(int col = 0; col < cols; col++)
				{
					string s = (char*)sqlite3_column_text(statement, col);
                    iss.str(s);
                    iss >> tmp;
                    data.push_back(tmp);
                    iss.clear();
				}

			}
			else
			{
				break;
			}
		}
        sqlite3_finalize(statement);
        dtest->SetID(data[0]);
        dtest->SetTestID(data[1]);
        dtest->SetTime(data[2]);
        dtest->SetWeight(data[3]);
	}
    return dtest;
}
void Database::DodajKolejnyDataTest(int id_testu, int tm , int wg)
{
    string s_id_testu, s_tm, s_wg, querry;
    sqlite3_stmt *statement;

    ostringstream ss;
    ss << id_testu;
    s_id_testu = ss.str();
    ostringstream ss2;
    ss2 << tm;
    s_tm = ss2.str();
    ostringstream ss3;
    ss3 << wg;
    s_wg = ss3.str();

    querry = "INSERT INTO data_test(test_id, time, weight) VALUES("+s_id_testu+","+s_tm+","+s_wg+");";
    if(sqlite3_prepare_v2(database, querry.c_str(), -1, &statement, 0) == SQLITE_OK)
	{
		int result = 0;
        result = sqlite3_step(statement);
		sqlite3_finalize(statement);
	}
}
Data_test* Database::ZwrocDataTest(int id)
{
    sqlite3_stmt *statement;
    string s_id, querry;
    ostringstream ss;
    ss << id;
    s_id = ss.str();
    Data_test* dtest = new Data_test();

    querry = "SELECT * FROM data_test WHERE id = "+s_id+";";

    if(sqlite3_prepare_v2(database, querry.c_str(), -1, &statement, 0) == SQLITE_OK)
	{
        int cols = sqlite3_column_count(statement);
		int result = 0;
		vector<int> data;
		int tmp;
		stringstream iss;
		while(true)
		{
			result = sqlite3_step(statement);

			if(result == SQLITE_ROW)
			{
				for(int col = 0; col < cols; col++)
				{
					string s = (char*)sqlite3_column_text(statement, col);
                    iss.str(s);
                    iss >> tmp;
                    data.push_back(tmp);
                    iss.clear();
				}
			}
			else
			{
				break;
			}
		}
        sqlite3_finalize(statement);
        dtest->SetID(data[0]);
        dtest->SetTestID(data[1]);
        dtest->SetTime(data[2]);
        dtest->SetWeight(data[3]);
	}
    return dtest;
}
vector<Test*> Database::ZwrocWszystkieTesty()
{
    vector<Test*> testy;
    vector<Data_test*> data_testy;
    sqlite3_stmt *statement;


    if(sqlite3_prepare_v2(database, "SELECT * FROM data_test;", -1, &statement, 0) == SQLITE_OK)
	{
        int cols = sqlite3_column_count(statement);
		int result = 0;
		vector<int> data;
		int tmp;
		stringstream iss;
		while(true)
		{
			result = sqlite3_step(statement);

			if(result == SQLITE_ROW)
			{
				for(int col = 0; col < cols; col++)
				{
					string s = (char*)sqlite3_column_text(statement, col);
                    iss.str(s);
                    iss >> tmp;
                    data.push_back(tmp);
                    iss.clear();
				}
                data_testy.push_back(new Data_test(data[0],data[1],data[2],data[3]));
                data.clear();
			}
			else
			{
				break;
			}
		}
        sqlite3_finalize(statement);
	}
    if(sqlite3_prepare_v2(database, "SELECT * FROM test;", -1, &statement, 0) == SQLITE_OK)
	{
        int cols = sqlite3_column_count(statement);
		int result = 0;
		int tmp_id;
		string tmp_start, tmp_finish;
		stringstream iss;
		while(true)
		{
			result = sqlite3_step(statement);

			if(result == SQLITE_ROW)
			{
                string s = (char*)sqlite3_column_text(statement, 0);
                iss.str(s);
                iss >> tmp_id;
                tmp_start = (char*)sqlite3_column_text(statement, 1);
                tmp_finish = (char*)sqlite3_column_text(statement, 2);

                testy.push_back(new Test(tmp_id, tmp_start, tmp_finish));
			}
			else
			{
				break;
			}
		}
        sqlite3_finalize(statement);
	}
	for(int i =0; i < testy.size();i++)
	{
	    for(int j =0 ; j < data_testy.size();j++)
	    {
	        if(testy[i]->GetID() == data_testy[j]->GetTestID())
	        {
	            testy[i]->AddDataTest(data_testy[j]);
	        }
	    }
	}
    return testy;
}


