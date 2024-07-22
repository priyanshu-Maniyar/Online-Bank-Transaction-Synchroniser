#include "declarations.h"
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <pthread.h>

sqlite3 *db;

void initializeDatabase() {
    char *err_msg = 0;
    int rc = sqlite3_open("banking_system.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    char *sql = "CREATE TABLE IF NOT EXISTS NormalUser ("
                "Id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "Username TEXT NOT NULL, "
                "Password TEXT NOT NULL, "
                "Balance INTEGER NOT NULL);"
                "CREATE TABLE IF NOT EXISTS JointUser ("
                "Id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "Keyword TEXT NOT NULL UNIQUE, "
                "Username1 TEXT NOT NULL, "
                "Password1 TEXT NOT NULL, "
                "Username2 TEXT NOT NULL, "
                "Password2 TEXT NOT NULL, "
                "Balance INTEGER NOT NULL);"
                "CREATE TABLE IF NOT EXISTS AdminUser ("
                "Id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "Username TEXT NOT NULL, "
                "Password TEXT NOT NULL);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(1);
    }
}

void cleanupDatabase() {
    sqlite3_close(db);
}

void* server_to_client(void* arg) {
    // Your existing server_to_client code
}

int main() {
    initializeDatabase();
    
    // Your existing server setup code

    cleanupDatabase();
    return 0;
}
