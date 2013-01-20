#ifndef TOOLS_H
#define TOOLS_H

#include <windows.h>
#include "Database.h"
#include "client.h"


void writeText(HWND hwnd, int x,int y, LPCSTR message,int color = 0x00000000);

void rewriteWindowThread(HWND hwnd);
void startRefresh();
void stopRefresh();

extern bool started ;
extern HANDLE rewriteHandle;
extern HWND childhwnd;
extern Database database ;
#endif
