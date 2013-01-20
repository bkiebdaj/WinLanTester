#include "tools.h"


HANDLE rewriteHandle;
void writeText(HWND hwnd, int x,int y, LPCSTR message,int color){
               RECT rect;
               HDC wdc = GetWindowDC(hwnd);
               GetClientRect (hwnd, &rect) ;
               SetTextColor(wdc, color);
                SetBkMode(wdc,TRANSPARENT);
                rect.left=x;
                rect.top=y;
                DrawText( wdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP  ) ;
                DeleteDC(wdc);
}

bool started = false ;
DWORD WINAPI rewriteWindowThread(LPVOID param){
    while(started)
    {
        Sleep(1000);
        cout<<"dziala"<<endl;
        InvalidateRect(childhwnd,NULL,true);
        RedrawWindow(childhwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    }
    return 0;
}

void startRefresh(){
    if(!started){
        started = true ;
        rewriteHandle = CreateThread(NULL,0,rewriteWindowThread,NULL,0,NULL);

    }
}

void stopRefresh(){
    started = false ;
    WaitForSingleObject(rewriteHandle,1000);
}
