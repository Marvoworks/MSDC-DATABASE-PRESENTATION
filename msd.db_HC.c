#include <stdlib.h>
#include <sqlite3.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "msd.h"

extern void drawData(char *stm);

void execute_sql(sqlite3 *db,  char *zSql, sqlite3_callback xCallback, void *pArg)
{
         if(!db)
         {
             MessageBox(hwndMain, "Please first perform DBCONNECTION", "SQL EXECUTOR ERROR", MB_ICONERROR | MB_OK);   
         } else {
     
         int  rc;
         char err[100];
         
         char *pzErrMsg = 0;
         
         rc = sqlite3_exec(db, zSql, xCallback, pArg, &pzErrMsg);
         
         if( rc != SQLITE_OK )
         {
          
         wsprintf(err, "SQL error:  %s\n", pzErrMsg);
         
         MessageBox(hwndMain, err, "SQL EXECUTOR ERROR", MB_ICONERROR | MB_OK);    
                      
             sqlite3_free(pzErrMsg);
         } else {
                     char stm[200];
                     
                     wsprintf(stm, "\r\nSQL Statement Executed Successfully.\r\n", zSql); 
                     
                     drawData(stm);  
                } 
         }   
}

void connectDB(sqlite3 **db)
{
         int  rc;
         
         rc = sqlite3_open("msd.org.db", db);
         if( rc )
         {             
             MessageBox(hwndMain, "Database Connection Setup unsuccessful", "SQL CONNECTOR", MB_ICONINFORMATION|MB_OK);              
                          
         }else
         {
               HWND SRES = GetDlgItem(hwndMain, ID_SQL_RESULT);
             
               char *stm = "\r\nDatabase Connection Setup successful.\r\n"; 
               drawData(stm);                                                                                      
         }
}
