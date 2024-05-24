#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

#define DATABASE_PATH "/srv/anime.db"
#define TABLE_TEMPLATE "templates/table.html"

void print_table(sqlite3* database);

int main(void) {
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

	FILE* template = fopen(TABLE_TEMPLATE, "r");

	if(!template) {
		printf("Status: 500\n");
		printf("Content-Type: text/plain\n\n");
		printf("Template error\n");

		return 1;
	}

	printf("Content-Type: text/html\n\n");

	const int BUFF_SIZE = 1024;
	char buff[BUFF_SIZE];
	const char* tag = "<c/>";

	while(fgets(buff, BUFF_SIZE, template)) {
		if(!strstr(buff, tag)) {
			printf(buff);

			continue;
		}

		print_table(database);
	}

	fclose(template);
	sqlite3_close(database);

	return 0;
}

void print_table(sqlite3* database) {
	int result;
	sqlite3_stmt* query;

	result = sqlite3_prepare_v2(database, "SELECT id, name, rating FROM anime", -1, &query, NULL);

	if(result != SQLITE_OK) {
		sqlite3_finalize(query);
		sqlite3_close(database);

		return;
	}

	printf(
		"<table>"
		"<tr>"
		"<th>ID</th>"
		"<th>Name</th>"
		"<th>Rating</th>"
		"</tr>"
	);

	while(sqlite3_step(query) == SQLITE_ROW) {
		printf(
			"<tr><td>%d</td><td>%s</td><td>%d</td></tr>",
			sqlite3_column_int(query, 0),
			sqlite3_column_text(query, 1),
			sqlite3_column_int(query, 2)
		);
	}

	printf("</table>");

	sqlite3_finalize(query);
}
