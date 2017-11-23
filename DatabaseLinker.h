#ifndef DATABASE_LINKER_H
#define DATABASE_LINKER_H

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string>
#include <vector>
using namespace std;

class DatabaseLinker{
    
public:
    DatabaseLinker();
    static int callback(void *NotUsed, int argc, char **argv, char **azColName);
    void createTable();
    void insert(string username, string password, string IP, bool online, string token);
    void remove();
    string retreive( string var, string toRetreive);
    void update( string var, string var_to_update, string newVal);
    vector<string> getOnline( string toRetreive);
    ~DatabaseLinker();
private:
    sqlite3 *db;
    char *zErrMsg;
    int rc;
    string sql;
    string C = "'";
    using Record = vector<string>;
    using Records = vector<Record>;
   };

#endif /* DatabaseLinker_hpp */
