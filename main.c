#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

#include "cgiutils.h"
#include "handles.h"
#include "database.h"

#define DATABASE_PATH "/srv/http/anime.db"

int main(void) {
	sqlite3* database;
	int result;

	result = sqlite3_open(DATABASE_PATH, &database);

	if(result != SQLITE_OK) {
		sqlite3_close(database);

		server_error("Database error");

		return 1;
	}

	if(!create_table(database)) {
		sqlite3_close(database);

		server_error("Table creation error");

		return 1;
	}

	char* method = getenv("REQUEST_METHOD");

	if(!method) {
		sqlite3_close(database);

		server_error("Empty request method");

		return 1;
	}

	if(strcmp(method, "GET") == 0) {
		handle_get(database);
	}else if(strcmp(method, "POST") == 0) {
		handle_post(database);
	}else {
		server_error("Invalid request method");
	}

	sqlite3_close(database);

	return 0;
}
