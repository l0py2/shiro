#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#define DATABASE_PATH "/srv/anime.db"

int main(int argc, char** argv) {
	sqlite3* database;
	int result;

	result = sqlite3_open(DATABASE_PATH, &database);

	if(result != SQLITE_OK) {
		sqlite3_close(database);

		printf("Status: 500\n");
		printf("Content-Type: text/plain\n\n");
		printf("Database error\n");

		return 1;
	}

	printf("Content-Type: text/html\n\n");

	sqlite3_stmt* query;

	result = sqlite3_prepare_v2(database, "SELECT id, name, rating FROM anime", -1, &query, NULL);

	if(result != SQLITE_OK) {
		sqlite3_finalize(query);
		sqlite3_close(database);

		return 1;
	}

	printf("<ul>");

	while(sqlite3_step(query) == SQLITE_ROW) {
		printf(
			"<li>[%d] %s - %d</li>",
			sqlite3_column_int(query, 0),
			sqlite3_column_text(query, 1),
			sqlite3_column_int(query, 2)
		);
	}

	printf("</ul>");

	sqlite3_finalize(query);
	sqlite3_close(database);

	return 0;
}
