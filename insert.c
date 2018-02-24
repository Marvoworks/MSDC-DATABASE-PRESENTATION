#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

#include "msd.h"
#include "insert.h"

extern LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

extern void execute_sql(sqlite3 *db,  char *zSql, sqlite3_callback xCallback, void *pArg);

extern void drawData(char *stm);

HWND hDlg;

static int selectFN(void *data, int argc, char **argv, char **azColname)
{  
     
     char *stm = malloc(100);
     memset(stm, '\0', 100);      
      
     int i;
     for(i=0; i<argc; i++)
     {                             
         wsprintf(stm, "%s", argv[i] ? argv[i] : "NULL");
         SetWindowText(GetDlgItem(hDlg, IDE_SF_FN), stm);                 
     }                                               
     
     return 0; 
}
static int selectLN(void *data, int argc, char **argv, char **azColname)
{ 
     
     char *stm = malloc(100);
     memset(stm, '\0', 100);      
       
     int i;
     for(i=0; i<argc; i++)
     {                             
         wsprintf(stm, "%s", argv[i] ? argv[i] : "NULL");  
         SetWindowText(GetDlgItem(hDlg, IDE_SF_LN), stm);        
     } 
     
     return 0; 
}
static int selectUN(void *data, int argc, char **argv, char **azColname)
{ 
     
     char *stm = malloc(100);
     memset(stm, '\0', 100);      
       
     int i;
     for(i=0; i<argc; i++)
     {                             
         wsprintf(stm, "%s", argv[i] ? argv[i] : "NULL");  
         SetWindowText(GetDlgItem(hDlg, IDE_SF_UN), stm);        
     } 
     
     return 0; 
}
static int selectEF(void *data, int argc, char **argv, char **azColname)
{   
     char *stm = malloc(400);
     memset(stm, '\0', 400);      
       
     int i;
     for(i=0; i<argc; i++)
     {                             
         wsprintf(stm, "%s", argv[i] ? argv[i] : "NULL");  
         SetWindowText(GetDlgItem(hDlg, IDE_SF_EF), stm);        
     } 
     
     return 0; 
}
static int selectPF(void *data, int argc, char **argv, char **azColname)
{  
     
     char *stm = malloc(200);
     memset(stm, '\0', 200);      
       
     int i;
     for(i=0; i<argc; i++)
     {                             
         wsprintf(stm, "%s", argv[i] ? argv[i] : "NULL");  
         SetWindowText(GetDlgItem(hDlg, IDE_SF_PF), stm);        
     } 
     
     return 0; 
}
void search_sql(sqlite3 *db,  char *zSql, sqlite3_callback xCallback, void *pArg)
{
         if(!db)
         {
             MessageBox(hwndMain, "Please first perform DBCONNECTION", "SQL EXECUTOR ERROR", MB_ICONERROR | MB_OK);   
         }else 
         {
     
         int  rc;
         char err[100];
         
         char *pzErrMsg = 0;
         
         rc = sqlite3_exec(db, zSql, xCallback, pArg, &pzErrMsg);
         
         if( rc != SQLITE_OK )
         {
          
         wsprintf(err, "SQL error:  %s\n", pzErrMsg);
         
         MessageBox(hwndMain, err, "SQL EXECUTOR ERROR", MB_ICONERROR | MB_OK);    
                      
             sqlite3_free(pzErrMsg);
         }

         }   
}

LRESULT CALLBACK InsertDLGProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
      switch(uMsg)
      {              
      case WM_INITDIALOG:
           {        
               hDlg = hwnd;     
                                                                                         
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
                                   
           }             
           break;
      case WM_COMMAND:
           switch (wParam)
           {
                  case IDCANCEL:
                       EndDialog(hwnd, TRUE);
                       break;
                       
                  case IDE_SF_SID:
                       {
                          BOOL bSuccess;        
                          int id =  GetDlgItemInt(hwnd, IDE_SF_ID, &bSuccess, FALSE);
                          if(bSuccess == TRUE)
                          {
                             char fn[300];
                             wsprintf(fn, "select firstname from developer where id=%d;", id);
                             search_sql(db, fn, selectFN, 0);
                             char ln[300];
                             wsprintf(ln, "select lastname from developer where id=%d;", id);
                             search_sql(db, ln, selectLN, 0);
                             char un[300];
                             wsprintf(un, "select username from developer where id=%d;", id);
                             search_sql(db, un, selectUN, 0);
                             char ef[400];
                             wsprintf(ef, "select email from developer where id=%d;", id);
                             search_sql(db, ef, selectEF, 0); 
                             char pf[300];
                             wsprintf(pf, "select password from developer where id=%d;", id); 
                             search_sql(db, pf, selectPF, 0);                                   
                                                                                                                            
                          }else
                          {
                             MessageBox(hwnd, "Invalid data purchased in ID Field", "Developer Form", MB_OK|MB_ICONINFORMATION);  
                          }        
                       }
                       break;     
                       
                  case IDE_SF_EB:
                  {
                       BOOL bSuccess;
                       int FNL, LNF, UNF, EF, PF;
                       char *firstname; char *lastname; char *username; char *email; char *password; 
                       
                       FNL = GetWindowTextLength(GetDlgItem(hwnd, IDE_SF_FN));
                       LNF = GetWindowTextLength(GetDlgItem(hwnd, IDE_SF_LN));
                       UNF = GetWindowTextLength(GetDlgItem(hwnd, IDE_SF_UN));
                       EF = GetWindowTextLength(GetDlgItem(hwnd,  IDE_SF_EF));
                       PF = GetWindowTextLength(GetDlgItem(hwnd,  IDE_SF_PF));
                       
                       int id =  GetDlgItemInt(hwnd, IDE_SF_ID, &bSuccess, FALSE);
                       if(bSuccess == TRUE)
                       {
                           if(FNL > 0)
                           {        
                             firstname = (char*)GlobalAlloc(GPTR, FNL + 1);
                             GetDlgItemText(hwnd, IDE_SF_FN, firstname,  FNL + 1);
                           }else
                           {
                              MessageBox(hwnd, "Nothing is purchased in Firstname Field", "Developer Form", MB_OK|MB_ICONINFORMATION);
                              goto EndSession;   
                           }
                           if(LNF > 0)
                           {        
                             lastname = (char*)GlobalAlloc(GPTR, LNF + 1);
                             GetDlgItemText(hwnd, IDE_SF_LN, lastname,  LNF + 1);
                           }else
                           {
                              MessageBox(hwnd, "Nothing is purchased in Lastname Field", "Developer Form", MB_OK|MB_ICONINFORMATION);
                              goto EndSession;   
                           }
                           if(UNF > 0)
                           {        
                             username = (char*)GlobalAlloc(GPTR, UNF + 1);
                             GetDlgItemText(hwnd, IDE_SF_UN, username,  UNF + 1);
                           }else
                           {
                              MessageBox(hwnd, "Nothing is purchased in Username Field", "Developer Form", MB_OK|MB_ICONINFORMATION);
                              goto EndSession;   
                           }
                           if(EF > 0)
                           {        
                             email = (char*)GlobalAlloc(GPTR, EF + 1);
                             GetDlgItemText(hwnd, IDE_SF_EF, email,  EF + 1);
                           }else
                           {
                              MessageBox(hwnd, "Nothing is purchased in Email Field", "Developer Form", MB_OK|MB_ICONINFORMATION);
                              goto EndSession;   
                           }
                           if(PF > 0)
                           {        
                             password = (char*)GlobalAlloc(GPTR, PF + 1);
                             GetDlgItemText(hwnd, IDE_SF_FN, password,  PF + 1);
                           }else
                           {
                              MessageBox(hwnd, "Nothing is purchased in Password Field", "Developer Form", MB_OK|MB_ICONINFORMATION);
                              goto EndSession;   
                           }                         
                           
                           char query[600];
                           
   wsprintf(query, "insert into developer values(%d, '%s', '%s', '%s', '%s', '%s');", id, firstname, lastname, username, email, password);
        
                           execute_sql(db, query, 0, 0);
                           
                           Sleep(2500);
                           
                           EndDialog(hwnd, TRUE);
                           
                                                                                                                                                      
                       }
                       else
                       {
                           MessageBox(hwnd, "Invalid data purchased in ID Field", "Developer Form", MB_OK|MB_ICONINFORMATION);
                       }                                               
                  }                                                   
                       break;
                  case IDE_SF_UBF:
                  {
                       BOOL bSuccess;
                       int FNL, LNF, UNF, EF, PF;
                       char *firstname; char *lastname; char *username; char *email; char *password; 
                       
                       FNL = GetWindowTextLength(GetDlgItem(hwnd, IDE_SF_FN));
                       LNF = GetWindowTextLength(GetDlgItem(hwnd, IDE_SF_LN));
                       UNF = GetWindowTextLength(GetDlgItem(hwnd, IDE_SF_UN));
                       EF = GetWindowTextLength(GetDlgItem(hwnd,  IDE_SF_EF));
                       PF = GetWindowTextLength(GetDlgItem(hwnd,  IDE_SF_PF));
                       
                       int id =  GetDlgItemInt(hwnd, IDE_SF_ID, &bSuccess, FALSE);
                       if(bSuccess == TRUE)
                       {
                           if(FNL > 0)
                           {        
                             firstname = (char*)GlobalAlloc(GPTR, FNL + 1);
                             GetDlgItemText(hwnd, IDE_SF_FN, firstname,  FNL + 1);
                           }else
                           {
                              MessageBox(hwnd, "Nothing is purchased in Firstname Field", "Developer Form", MB_OK|MB_ICONINFORMATION);
                              goto EndSession;   
                           }
                           if(LNF > 0)
                           {        
                             lastname = (char*)GlobalAlloc(GPTR, LNF + 1);
                             GetDlgItemText(hwnd, IDE_SF_LN, lastname,  LNF + 1);
                           }else
                           {
                              MessageBox(hwnd, "Nothing is purchased in Lastname Field", "Developer Form", MB_OK|MB_ICONINFORMATION);
                              goto EndSession;   
                           }
                           if(UNF > 0)
                           {        
                             username = (char*)GlobalAlloc(GPTR, UNF + 1);
                             GetDlgItemText(hwnd, IDE_SF_UN, username,  UNF + 1);
                           }else
                           {
                              MessageBox(hwnd, "Nothing is purchased in Username Field", "Developer Form", MB_OK|MB_ICONINFORMATION);
                              goto EndSession;   
                           }
                           if(EF > 0)
                           {        
                             email = (char*)GlobalAlloc(GPTR, EF + 1);
                             GetDlgItemText(hwnd, IDE_SF_EF, email,  EF + 1);
                           }else
                           {
                              MessageBox(hwnd, "Nothing is purchased in Email Field", "Developer Form", MB_OK|MB_ICONINFORMATION);
                              goto EndSession;   
                           }
                           if(PF > 0)
                           {        
                             password = (char*)GlobalAlloc(GPTR, PF + 1);
                             GetDlgItemText(hwnd, IDE_SF_PF, password,  PF + 1);
                           }else
                           {
                              MessageBox(hwnd, "Nothing is purchased in Password Field", "Developer Form", MB_OK|MB_ICONINFORMATION);
                              goto EndSession;   
                           }                         
                           
                           char query[700];
                           
   wsprintf(query, "update developer set firstname='%s', lastname='%s', username='%s',email='%s', password='%s' where id=%d;", firstname, lastname, username, email, password, id);
        
                           execute_sql(db, query, 0, 0);
                           
                           Sleep(2500);
                           
                           EndDialog(hwnd, TRUE);
                           
                                                                                                                                                      
                       }
                       else
                       {
                           MessageBox(hwnd, "Invalid data purchased in ID Field", "Developer Form", MB_OK|MB_ICONINFORMATION);
                       }                                               
                  }                                                   
                       break;                       
           }
           break;
      case WM_CLOSE:
                   EndDialog(hwnd, TRUE);
           break;
      case WM_CTLCOLORDLG:
             {
               HBRUSH g_hbrBackground = CreateSolidBrush(RGB(150, 100, 70));                
               HDC hdcStatic = (HDC)wParam;
               SetTextColor(hdcStatic, RGB(255, 255, 255));
               SetBkMode(hdcStatic, TRANSPARENT);
               return (LONG)g_hbrBackground;
            }               
            break;  
       case WM_CTLCOLORSTATIC:
             {
               HBRUSH g_hbrBackground = CreateSolidBrush(RGB(150, 100, 70));                
               HDC hdcStatic = (HDC)wParam;
               SetTextColor(hdcStatic, RGB(255, 255, 255));
               SetBkMode(hdcStatic, TRANSPARENT);
               return (LONG)g_hbrBackground;
            } 
             break;     
             
      EndSession:
                 MessageBox(hwnd, "\r\nPlease purchase form data following labels.\r\nNo field should be left blank.\r\n", "Developer Form", MB_OK|MB_ICONINFORMATION);             
                             
      }
      return FALSE;
}

