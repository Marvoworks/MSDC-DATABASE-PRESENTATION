#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <sqlite3.h>
#include "msd.h"


void createControls(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
   HWND SQLQU_INST, SQLRESULT, SQL_BTN; HFONT hFont; HANDLE g_Inst;  
     
   SQLQU_INST = CreateWindow("Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 62, 55, 355, 23, hwnd, (HMENU) ID_SQL_QUERY, NULL, NULL);             
   hFont = CreateFont(16, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0, 0,0,0,0, "Tahoma");
   SendDlgItemMessage(hwnd, ID_SQL_QUERY, WM_SETFONT, (WPARAM)hFont, TRUE);
   
   LPTSTR szN = "Please first connect, erase me and then insert SQL Statement here.";
   SetWindowText(SQLQU_INST, szN);   
   
   SQLRESULT = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_BORDER | ES_AUTOVSCROLL | WS_VSCROLL, 62, 87, 452, 250, hwnd, (HMENU) ID_SQL_RESULT, NULL, NULL);             
   SendDlgItemMessage(hwnd, ID_SQL_RESULT, WM_SETFONT, (WPARAM)hFont, TRUE); 
   
   SQL_BTN = CreateWindow("Button", "Execute", WS_CHILD | WS_VISIBLE , 420, 55, 95, 23, hwnd, (HMENU)ID_SQL_QUERY_BTN, g_Inst, NULL);
   SendDlgItemMessage(hwnd, ID_SQL_QUERY_BTN, WM_SETFONT, (WPARAM)hFont, TRUE);            
}
