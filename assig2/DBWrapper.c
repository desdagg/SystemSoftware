#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>

#include "DBWrapper.h"

sqlite3 *db;
char db_name[] = "dt228.db";

//int getDateTime(char **currentDate);

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int execute(char *sql) {
  int rc;
  char *zErrMsg = 0;
  fprintf(stdout, "[DB] SQL : %s \n", sql);
  /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s %d\n", zErrMsg , rc);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "[DB] Operation successful\n");
   }
}
  
int createUserTable(int fd){
  char *sql;
  /* Create SQL statement */
   sql = "CREATE TABLE USERS("  \
         "ID INT PRIMARY KEY     NOT NULL," \
         "NAME           TEXT    NOT NULL," \
         "PASSWD         TEXT );";

   execute(sql);
}

int createLogTable(int fd){
  char *sql;
  sql = "CREATE TABLE LOGS(" \
        "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
        "USER TEXT NOT NULL," \
        "FILE_CHANGED TEXT NOT NULL," \
        "DATE TEXT);";

  execute(sql);
}

int addUsers(){
  char *sql;
  sql = "INSERT INTO USERS (ID, NAME,PASSWD) " \
    " VALUES(1,'Des','pass');" \
  "INSERT INTO USERS (ID, NAME,PASSWD) " \
    "VALUES(2, 'Bob','bob');" \
  "INSERT INTO USERS (ID,NAME,PASSWD) " \
    "VALUES(3, 'Dave','dave');" ;

  execute(sql);
}
  

int connectToDb(){
  int result; 
  /* Open database */
  result = sqlite3_open(db_name, &db);
   
   if( result ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stdout, "[DB] Opened database successfully\n");
   }
   return result;
}



int doesExist(char *sql){
   struct sqlite3_stmt *selectCmd;
  int response = 0;  //  Default to no record found;
 
  int result =sqlite3_prepare_v2(db, sql,-1 , &selectCmd,NULL);
  if(result == SQLITE_OK)
    {
      if (sqlite3_step(selectCmd) == SQLITE_ROW)	  
	  response= 1; 
      sqlite3_finalize(selectCmd);
    }
  return response; 
}

void init(){
  int rc;
  char *checkTableSql = "SELECT name FROM sqlite_master WHERE type='table' AND name ='USERS';";  
  char *checkLogTableSql = "SELECT name FROM sqlite_master WHERE type='table' AND name='LOGS';";
  rc = connectToDb();
  sqlite3_busy_timeout(db,600);

  if (doesExist(checkTableSql)){    
    fprintf(stdout, "[DB] USERS Table Exists\n");
  }else {
     fprintf(stdout, "USERS Table Does not Exist\n");
      //  Create the USERS database table
     createUserTable(rc);
     /// Add the list of default users allowed to administer the Website  
     addUsers();
  }
  if (doesExist(checkLogTableSql)){
    fprintf(stdout, "[DB] LOGS Table Exists\n");
  }else {
    fprintf(stdout, "LOGS Table Does not Exist");
    createLogTable(rc);

  }
}

int authenticate(char *name, char *password){
  char sql[80];
  strcpy(sql, "SELECT * FROM USERS WHERE name ='");
  strcat(sql,name);
  strcat(sql,"' AND PASSWD='");
  strcat(sql,password);
  strcat(sql,"' ;");
  fprintf(stdout, "SQL : %s\n" , sql);

  return doesExist(sql);
}


void logFileChanges(char **userName, char **targetFilePath){
  char sql[300];
  char *currentDate;

time_t current_time;
  char *c_time_string;

  current_time = time(NULL);

  if(current_time == ((time_t)-1)){
    (void) fprintf(stderr, "*TIME* Failure to obtain the current time\n");
   
  }

  c_time_string = ctime(&current_time);

  if (c_time_string == NULL){
    (void) fprintf(stderr, "*TIME* Failure to convert the time \n" );
  }

    //get date as string
    strcpy(sql, "INSERT INTO LOGS (USER,FILE_CHANGED,DATE) VALUES('");
    strcat(sql, *userName);
    strcat(sql, "','");
    strcat(sql, *targetFilePath);
    strcat(sql, "','");
    strcat(sql, c_time_string);
    strcat(sql, "');");
    execute(sql);
  
}

void shutdownDB(){
   sqlite3_close(db);
}

