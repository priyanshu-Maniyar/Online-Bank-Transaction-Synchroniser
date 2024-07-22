#include "declarations.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sqlite3.h>

extern sqlite3 *db;

// Helper function to execute a SQL query
int executeSQL(const char *sql, sqlite3_stmt **stmt) {
    int rc = sqlite3_prepare_v2(db, sql, -1, stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute SQL query: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    return 0;
}

// ---------- Search related operations --------------
long int balanceEnquiry(int id, int type) {
    long int bal = -1;
    sqlite3_stmt *stmt;
    const char *sql;
    
    if (type == NORMAL_USER) {
        sql = "SELECT Balance FROM NormalUser WHERE Id = ?";
        if (executeSQL(sql, &stmt) != 0) return -1;
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            bal = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else if (type == JOINT_USER) {
        sql = "SELECT Balance FROM JointUser WHERE Id = ?";
        if (executeSQL(sql, &stmt) != 0) return -1;
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            bal = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    return bal;
}

struct Data viewDetailsNorm(int id) {
    struct Data dt;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT Username, Password, Balance FROM NormalUser WHERE Id = ?";

    if (executeSQL(sql, &stmt) != 0) return dt;

    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        strncpy(dt.user.username, (const char *)sqlite3_column_text(stmt, 0), MAX_CHAR_LEN - 1);
        strncpy(dt.user.password, (const char *)sqlite3_column_text(stmt, 1), MAX_CHAR_LEN - 1);
        dt.balance = sqlite3_column_int(stmt, 2);
        dt.id = id;
    }
    sqlite3_finalize(stmt);

    return dt;
}

struct JointData viewDetailsJoint(int id) {
    struct JointData jdt;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT Keyword, Username1, Password1, Username2, Password2, Balance FROM JointUser WHERE Id = ?";

    if (executeSQL(sql, &stmt) != 0) return jdt;

    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        strncpy(jdt.keyword, (const char *)sqlite3_column_text(stmt, 0), MAX_CHAR_LEN - 1);
        strncpy(jdt.user1.username, (const char *)sqlite3_column_text(stmt, 1), MAX_CHAR_LEN - 1);
        strncpy(jdt.user1.password, (const char *)sqlite3_column_text(stmt, 2), MAX_CHAR_LEN - 1);
        strncpy(jdt.user2.username, (const char *)sqlite3_column_text(stmt, 3), MAX_CHAR_LEN - 1);
        strncpy(jdt.user2.password, (const char *)sqlite3_column_text(stmt, 4), MAX_CHAR_LEN - 1);
        jdt.balance = sqlite3_column_int(stmt, 5);
        jdt.id = id;
    }
    sqlite3_finalize(stmt);

    return jdt;
}

// ---------- Modify related operations -----------
int passwordChangeNorm(int id, char *password) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE NormalUser SET Password = ? WHERE Id = ?";

    if (executeSQL(sql, &stmt) != 0) return -1;

    sqlite3_bind_text(stmt, 1, password, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to update password: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);

    return 0;
}

int passwordChangeJoint(int id, char *password, char *cur_username) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE JointUser SET Password1 = CASE WHEN Username1 = ? THEN ? ELSE Password1 END, "
                      "Password2 = CASE WHEN Username2 = ? THEN ? ELSE Password2 END "
                      "WHERE Id = ?";

    if (executeSQL(sql, &stmt) != 0) return -1;

    sqlite3_bind_text(stmt, 1, cur_username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, cur_username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, password, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to update joint password: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);

    return 0;
}

int depositAmtNorm(int id, long int amt) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE NormalUser SET Balance = Balance + ? WHERE Id = ?";

    if (executeSQL(sql, &stmt) != 0) return -1;

    sqlite3_bind_int(stmt, 1, amt);
    sqlite3_bind_int(stmt, 2, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to deposit amount: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);

    return 0;
}

int depositAmtJoint(int id, long int amt) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE JointUser SET Balance = Balance + ? WHERE Id = ?";

    if (executeSQL(sql, &stmt) != 0) return -1;

    sqlite3_bind_int(stmt, 1, amt);
    sqlite3_bind_int(stmt, 2, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to deposit joint amount: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);

    return 0;
}

int withdrawAmtNorm(int id, long int amt) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE NormalUser SET Balance = Balance - ? WHERE Id = ? AND Balance >= ?";

    if (executeSQL(sql, &stmt) != 0) return -1;

    sqlite3_bind_int(stmt, 1, amt);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_bind_int(stmt, 3, amt);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to withdraw amount: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);

    return 0;
}

int withdrawAmtJoint(int id, long int amt) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE JointUser SET Balance = Balance - ? WHERE Id = ? AND Balance >= ?";

    if (executeSQL(sql, &stmt) != 0) return -1;

    sqlite3_bind_int(stmt, 1, amt);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_bind_int(stmt, 3, amt);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to withdraw joint amount: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);

    return 0;
}
