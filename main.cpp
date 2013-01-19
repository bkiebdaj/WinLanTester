#include <windows.h>
#include "Database.h"
#include "client.h"
#include "tools.h"
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "CodeBlocksWindowsApp";
char szClassNamePanel[ ] = "PanelWindow";
Database database ;
Test *actualTest = NULL ;
vector<Test*> test_lista;
int selectIndex = -1;

WNDCLASSEX wincl, panelcl;
HWND childhwnd,hwnd;
HWND startButton;
HWND stopButton;
HWND testsList;
Client klient;
int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{

    MSG messages;            /* Here messages to the application are saved */
      /* Data structure for the windowclass */


    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    panelcl.hInstance = hThisInstance;
    panelcl.lpszClassName = szClassNamePanel;
    panelcl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    panelcl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    panelcl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    panelcl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    panelcl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    panelcl.hCursor = LoadCursor (NULL, IDC_ARROW);
    panelcl.lpszMenuName = NULL;                 /* No menu */
    panelcl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    panelcl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    panelcl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    if (!RegisterClassEx (&panelcl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Code::Blocks Template Windows App",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           800,                 /* The programs width */
           650,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );


    childhwnd = CreateWindowEx(0,
                        szClassNamePanel,
                        TEXT("Panel"),
                        WS_CHILD | WS_VISIBLE | WS_BORDER,
                        10,
                        10,
                        580,
                        580,
                        hwnd,
                        NULL,
                        hThisInstance,
                        NULL);





    startButton = CreateWindowEx( 0, "BUTTON", "Start test", WS_CHILD | WS_VISIBLE,
        620, 20, 140, 30, hwnd, NULL, hThisInstance, NULL );

    stopButton = CreateWindowEx( 0, "BUTTON", "Stop test", WS_CHILD | WS_VISIBLE,
        620, 60, 140, 30, hwnd, NULL, hThisInstance, NULL );
    testsList = CreateWindowEx( WS_EX_CLIENTEDGE, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |WS_VSCROLL|
        CBS_DROPDOWN, 620, 100, 140, 150, hwnd, NULL, hThisInstance, NULL );

    test_lista = database.ZwrocWszystkieTesty();
    cout<<test_lista.size()<<endl;
    actualTest = database.ZwrocTest(9);
    for(int i = 0; i<test_lista.size(); i++){
        SendMessage( testsList, CB_ADDSTRING, 0,( LPARAM ) test_lista[i]->GetStart_time().c_str() );
    }


//--------Testowanie bazy----------------------
    //Narazie zakomentowalem do testow z klientem i serwerem.... Igor
    /*Database db;
    Test* testowy = db.StworzTest();
    cout << "ID Testowego: " << testowy->GetID() << endl;
    cout << "Start Date Testowego: " << testowy->GetStart_time() << endl;
    Data_test* data_testowy = db.ZwrocNowyDataTest();
    cout << "Nowy data_test"<<endl;
    cout << "Id: " << data_testowy->GetID() << " Test_id: " << data_testowy->GetTestID() << " Time: " << data_testowy->GetTime() << " Weight: " << data_testowy->GetWeight() <<endl;
    db.DodajKolejnyDataTest(1,44,55);
    data_testowy = db.ZwrocNowyDataTest();
    cout << "Nowy data_test"<<endl;
    cout << "Id: " << data_testowy->GetID() << " Test_id: " << data_testowy->GetTestID() << " Time: " << data_testowy->GetTime() << " Weight: " << data_testowy->GetWeight() <<endl;
    data_testowy = db.ZwrocDataTest(1);
    cout << "Pierwszy data_test"<<endl;
    cout << "Id: " << data_testowy->GetID() << " Test_id: " << data_testowy->GetTestID() << " Time: " << data_testowy->GetTime() << " Weight: " << data_testowy->GetWeight() <<endl;

    db.ZakonczTest(2);
    vector<Test*> testy = db.ZwrocWszystkieTesty();
    cout << "Start Date Pierwszego: " << testy[0]->GetStart_time() << endl;
*/
//--------------------------------------------

//--------------Testowanie klienta-----------





    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);
    ShowWindow( childhwnd, nCmdShow);
    UpdateWindow(childhwnd);
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }
    //klient.stop_test();

//zamykanie klienta
//-----------koniec testu klienta-----
    //klient.close();
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage() */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        PAINTSTRUCT ps;
        HDC hdc;
        case WM_PAINT:

            if(hwnd == childhwnd){

               hdc=BeginPaint(hwnd,&ps);
               MoveToEx(hdc,30,30,0);
               LineTo(hdc,30,540);
               LineTo(hdc,540,540);


               for(int i = 0 ; i<= 10 ; i++){
                    char temp[10] ;
                    itoa(i*100,temp,10);
                    writeText(hwnd,0,530 - ( i * 50 ),temp);
               }

               if(actualTest != NULL){

                    vector<Data_test*> data_test = actualTest->GetAllDataTests();

                    if(data_test.size() > 0){
                        MoveToEx(hdc,30,540,0);

                        int step = 510/data_test.size();

                        for(int i = 0; i<data_test.size();i++){
                            LineTo(hdc,30+(i+1)*step,540 - data_test[i]->GetWeight()/1024/2);
                        }
                    }


               }


               EndPaint(hwnd,&ps);

            }

            break;
        case WM_COMMAND:
            if(( HWND ) lParam == startButton ){
                if(klient.connect_to_host("localhost",100))
                klient.start_test();
                break;
            }
            if(( HWND ) lParam == stopButton ){
                klient.stop_test();
                break;
            }
            if(( HWND ) lParam == testsList ){
                int tempIndex = SendMessage(testsList,CB_GETCURSEL,0,0);
                if(selectIndex != tempIndex){
                    selectIndex = tempIndex ;
                    actualTest = database.ZwrocTest(selectIndex+1);
                    cout<<actualTest->GetID()<<endl;

                    InvalidateRect(childhwnd,NULL,true);
                    RedrawWindow(childhwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

                }
                break;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}


