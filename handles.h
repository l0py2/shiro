#ifndef HANDLES_H
#define HANDLES_H

#define MAIN_TEMPLATE "templates/main.html"

#include <sqlite3.h>

void handle_get(sqlite3* database);
void handle_post(sqlite3* database);
void handle_addition(sqlite3* database, char* input);
void handle_update(sqlite3* database, char* input);
void handle_delete(sqlite3* database, char* input);

#endif
