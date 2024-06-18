#include "database.h"

#include <sqlite3.h>
#include <stddef.h>

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
