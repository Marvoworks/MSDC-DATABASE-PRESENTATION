/* @Globals */
HINSTANCE hInstance;
HWND hwndMain;               /* This is the handle for our window */

/*Databse */
sqlite3 *db;

/* Sector for IMPORTANT EXE Defines */
#define HWND_ICO       100
#define MSDBUI_MAN            80  
#define MSDBUI_MENU      780

/*Sector for Controls*/
#define ID_SQL_QUERY   4040
#define ID_SQL_RESULT  4050
#define IDC_LIST       5050

#define IDC_STA_O     6060
#define IDC_STATIC    -1
#define ABOUT_DLG     6050

/*Sector for  commans */
#define ID_SQL_QUERY_BTN 2000
#define MSDBUI_DB_CONNECT   4060
#define MSDBUI_DB_DISCON    4080
#define MSDBUI_ABOUT  2020
#define MSDBUI_EXIT   2030
#define MEN_REST      5090

#define IDC_SQL_INSERTDATA     7000

#define MAX (50)
