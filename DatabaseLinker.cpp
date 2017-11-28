#include "DatabaseLinker.h"

DatabaseLinker:: DatabaseLinker(){
    createTable();
}

DatabaseLinker::~DatabaseLinker(){

}

int DatabaseLinker:: callback(void *NotUsed, int argc, char **argv, char **azColName){
    Records* records = static_cast<Records*>(NotUsed);
    try {
        records->emplace_back(argv, argv + argc);
    }
    catch (...) {
        // abort select on failure, don't let exception propogate thru sqlite3 call-stack
        return 1;
    }
    return 0;
}

void DatabaseLinker:: createTable(){
    rc = sqlite3_open("test.db", &db);
    //Create SQL statement
    sql = "CREATE TABLE USERS("
    "USERNAME VARCHAR(50) PRIMARY KEY    NOT NULL,"
    "PASSWORD VARCHAR(50)              NOT NULL,"
    "IP      VARCHAR(50),"
    "ONLINE      BOOL,"
    "TOKEN   VARCHAR(100));";
    const char* sqlChar = sql.c_str();
    rc = sqlite3_exec(db, sqlChar, callback, 0, &zErrMsg);
    sqlite3_close(db);
}

bool DatabaseLinker:: insert(string username, string password, string IP, bool online, string token){
    rc = sqlite3_open("test.db", &db);
    //Create SQL statement
    
    sql = "INSERT INTO USERS VALUES (" + C +username + C +"," + C+password+C + "," + C+IP+C + "," + C+to_string(online)+C+ "," +C+token+C + ");";
    const char* sqlChar = sql.c_str();
    rc = sqlite3_exec(db, sqlChar, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
         //fprintf(stderr, "SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
         return false;
     }

    sqlite3_close(db);
    return true;
}

string DatabaseLinker:: retreive(string var, string toRetreive){
    Records records;
    string password;
    rc = sqlite3_open("test.db", &db);
    sql = "SELECT " + toRetreive + " FROM USERS "
    "WHERE USERNAME ="+C+var+C+";";

    const char* sqlChar = sql.c_str();
    rc = sqlite3_exec(db, sqlChar, callback, &records, &zErrMsg);
    sqlite3_close(db);
    for (auto& record : records) {
        for ( const auto &s : record )
            password = s;
    }   // cout << &records << endl;
    return password;
}


void DatabaseLinker::update( string var, string var_to_update, string newVal){
    Records records;
    rc = sqlite3_open("test.db", &db);
    sql = "UPDATE USERS set "+var_to_update+" = "+ C+newVal+C+ " where USERNAME= "+C+var+C+";";
    
    const char* sqlChar = sql.c_str();
    rc = sqlite3_exec(db, sqlChar, callback, &records, &zErrMsg);
    sqlite3_close(db);

}

vector<string> DatabaseLinker::getOnline( string toRetreive){
    vector<string> list;
    Records records;
    rc = sqlite3_open("test.db", &db);
    sql = "SELECT " + toRetreive + " FROM USERS "
    "WHERE ONLINE = 'true';";
    
    const char* sqlChar = sql.c_str();
    rc = sqlite3_exec(db, sqlChar, callback, &records, &zErrMsg);
    sqlite3_close(db);
    
    for (auto& record : records) {
        for ( const auto &s : record )
            list.push_back(s);
    }   // cout << &records << endl;
    return list;
}

void DatabaseLinker:: remove(){
    
}
