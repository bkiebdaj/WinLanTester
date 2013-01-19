#include "tools.h"

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
