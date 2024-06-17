#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

#include "cgiutils.h"
#include "handles.h"

#define DATABASE_PATH "/srv/http/anime.db"

int create_table(sqlite3* database);

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

int create_table(sqlite3* database) {
	int result;
	sqlite3_stmt* query;

	result = sqlite3_prepare_v2(
		database,
		"CREATE TABLE IF NOT EXISTS anime ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"name VARCHAR(128) NOT NULL,"
		"rating INTEGER NOT NULL"
		")",
		-1,
		&query,
		NULL
	);

	if(result != SQLITE_OK) {
		sqlite3_finalize(query);

		return 0;
	}

	result = sqlite3_step(query);

	if(result != SQLITE_DONE) {
		sqlite3_finalize(query);

		return 0;
	}

	sqlite3_finalize(query);

	return 1;
}
