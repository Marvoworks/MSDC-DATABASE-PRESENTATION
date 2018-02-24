#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include "msd.h"
#include "insert.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AboutDLGProc(HWND, UINT, WPARAM, LPARAM);
extern LRESULT CALLBACK InsertDLGProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* EXTERN Functions */
extern void execute_sql(sqlite3 *db,  char *zSql, sqlite3_callback xCallback, void *pArg);
extern void connectDB(sqlite3 *db);
extern createControls(HWND hwnd, WPARAM wParam, LPARAM lParam);

UINT    ListBoxIndex;

void drawData(char *stm)
{ 
     HWND SRES = GetDlgItem(hwndMain, ID_SQL_RESULT);
     
             SendMessage(SRES, LB_INSERTSTRING, ListBoxIndex, (LPARAM)stm);
             SendMessage(SRES, LB_SETCURSEL, ListBoxIndex, 0);
             ListBoxIndex++;                                             
}

static int select_callback(void *data, int argc, char **argv, char **azColname)
{
     HWND SRES = GetDlgItem(hwndMain, ID_SQL_RESULT);  
     
     char *stm = malloc(500);
     memset(stm, '\0', 500);      
       
     int i;
     for(i=0; i<argc; i++)
     {                             
         wsprintf(stm, "%s:  %s", azColname[i], argv[i] ? argv[i] : "NULL");  
         drawData(stm);        
     } 
     
     return 0; 
}

/*  Make the class name into a global variable  */
char szClassName[ ] = "msdDB";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{                
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    
    hInstance = GetModuleHandle(NULL);

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    
    wincl.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(HWND_ICO));
    
    wincl.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(HWND_ICO), IMAGE_ICON, 16, 16, 0);
    
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(MSDBUI_MENU);                 
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(150, 100, 70));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
  hwndMain = CreateWindowEx (
           WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "MSD DATABASE PRESENTATION",       /* Title Text */
           WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           590,                 /* The programs width */
           450,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,             
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwndMain, nFunsterStil);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
             {
                       createControls(hwnd, wParam, lParam);
                       
             }
             break;
        case WM_SIZE:
             
             
             
             break;
             
        case WM_COMMAND:
             switch(LOWORD(wParam))
             {
                  case MSDBUI_EXIT:
                              SendMessage(hwnd, WM_CLOSE, 0, 0);
                       break;
                  case MSDBUI_DB_CONNECT:
                              connectDB(&db);
                       break;
                  case MEN_REST:
                             {
                               HWND SRES = GetDlgItem(hwnd, ID_SQL_RESULT);              
                               SendMessage(SRES, LB_RESETCONTENT, 0, 0);
                               ListBoxIndex = 0;            
                             }
                       break;     
                  case MSDBUI_ABOUT:
                       {
                              hInstance = GetModuleHandle(NULL);
    
                              if (DialogBox(hInstance, MAKEINTRESOURCE(ABOUT_DLG), NULL, (DLGPROC)AboutDLGProc) == -1) 
                              {
                                MessageBox(NULL, "Unable to create MSDLG about dialog!", "DialogBox failure", MB_ICONSTOP|MB_OK);
                              }                                     
                       }
                       break;
                  case IDC_SQL_INSERTDATA:
                       {
                              hInstance = GetModuleHandle(NULL);
    
                              if (DialogBox(hInstance, MAKEINTRESOURCE(SQL_INSERT), NULL, (DLGPROC)InsertDLGProc) == -1) 
                              {
                                MessageBox(NULL, "Unable to create MSDLG about dialog!", "DialogBox failure", MB_ICONSTOP|MB_OK);
                              }                                     
                       }
                       break;                            
                  case ID_SQL_QUERY_BTN:
                       {
                          HWND QST = GetDlgItem(hwnd, ID_SQL_QUERY); 
                          HWND SRES = GetDlgItem(hwnd, ID_SQL_RESULT);  
                                      
                          DWORD selQL = GetWindowTextLength(QST);
                          
                          if(selQL == 0)
                          {
                              char *stm = "Please insert SQL statement"; 
                              SendMessage(QST, WM_SETTEXT, 0, (LPARAM)stm);                                  
                          } else
                          {  
                              LPSTR pszTEXT;
                              pszTEXT = (LPSTR)GlobalAlloc(GPTR, selQL + 1);
                              if(pszTEXT != NULL)
                              {
                                         if(GetWindowText(QST, pszTEXT, selQL + 1))
                                         {
                                             execute_sql(db, pszTEXT, select_callback, 0);
                                         }
                                         GlobalFree(pszTEXT);
                              }                                                                                       
                          }
                                        
                       }
                       break;                 
             }
             break;
                 
        case WM_CLOSE:
                       SendMessage(hwnd, WM_DESTROY, 0, 0); 
             break;   
        case WM_DESTROY:
                       PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
                       return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK AboutDLGProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
      switch(uMsg)
      {              
      case WM_INITDIALOG:
           {                                                                             
               /* Giving dialog a icon */          
                         
               HICON hIcon, hIconSm;
               
               hInstance = GetModuleHandle(NULL);
               
               hIcon = LoadImage(hInstance, MAKEINTRESOURCE(HWND_ICO), IMAGE_ICON, 32, 32, 0);
               if(hIcon)
               {
                   SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);     
               } else
               {
                   MessageBox(hwnd, "Could not load large icon!", "Error", MB_OK|MB_ICONSTOP);  
               }
               hIconSm = LoadImage(hInstance, MAKEINTRESOURCE(HWND_ICO), IMAGE_ICON, 16, 16, 0); 
               if(hIconSm)
               {
                   SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);     
               } else
               {
                   MessageBox(hwnd, "Could not load small icon!", "Error", MB_OK|MB_ICONSTOP);  
               }   
               
               HFONT hFont = CreateFont(18, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0, 0,0,0,0, "Tahoma");
               SendDlgItemMessage(hwnd, IDC_STA_O, WM_SETFONT, (WPARAM)hFont, TRUE);
                                      
           }             
           break;
      case WM_COMMAND:
           switch (wParam)
           {
                  case IDOK:
                       EndDialog(hwnd, TRUE);
                       break;
           }
           break;
      case WM_CLOSE:
                   EndDialog(hwnd, TRUE);
           break;
      case WM_CTLCOLORDLG:
             {
               HBRUSH g_hbrBackground = CreateSolidBrush(RGB(255, 255, 255));                
               HDC hdcStatic = (HDC)wParam;
               SetTextColor(hdcStatic, RGB(0, 0, 0));
               SetBkMode(hdcStatic, TRANSPARENT);
               return (LONG)g_hbrBackground;
            }               
            break;  
       case WM_CTLCOLORSTATIC:
             {
               HBRUSH g_hbrBackground = CreateSolidBrush(RGB(255, 255, 255));                
               HDC hdcStatic = (HDC)wParam;
               SetTextColor(hdcStatic, RGB(100, 210, 145));
               SetBkMode(hdcStatic, TRANSPARENT);
               return (LONG)g_hbrBackground;
            } 
             break; 
       case WM_CTLCOLORBTN:
            {
                return (LRESULT)GetSysColorBrush(COLOR_WINDOW+4);           
            }
            break;                
      }
      return FALSE;
}
