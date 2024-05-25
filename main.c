#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

#define DATABASE_PATH "/srv/anime.db"
#define MAIN_TEMPLATE "templates/main.html"

void extract_parameters(int* limit, int* page);
void print_limit_element(int limit);
void print_page_element(int page);
void print_table(sqlite3* database, int limit, int page);

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

	FILE* template = fopen(MAIN_TEMPLATE, "r");

	if(!template) {
		printf("Status: 500\n");
		printf("Content-Type: text/plain\n\n");
		printf("Template error\n");

		return 1;
	}

	int limit = 5;
	int page = 1;

	extract_parameters(&limit, &page);

	printf("Content-Type: text/html\n\n");

	const int BUFF_SIZE = 1024;
	char buff[BUFF_SIZE];
	const char* limit_tag = "<c limit/>";
	const char* page_tag = "<c page/>";
	const char* table_tag = "<c table/>";

	while(fgets(buff, BUFF_SIZE, template)) {
		if(strstr(buff, limit_tag)) {
			print_limit_element(limit);
		}else if(strstr(buff, page_tag)) {
			print_page_element(page);
		}else if(strstr(buff, table_tag)) {
			print_table(database, limit, page);
		}else {
			printf(buff);
		}
	}

	fclose(template);
	sqlite3_close(database);

	return 0;
}

void extract_parameters(int* limit, int* page) {
	char* query_str = getenv("QUERY_STRING");

	if(!query_str)
		return;

	char* token = strtok(query_str, "&");

	while(token) {
		sscanf(token, "limit=%d", limit);
		sscanf(token, "page=%d", page);

		token = strtok(NULL, "&");
	}
}

void print_limit_element(int limit) {
	printf(
		"<label>Limit: "
		"<input value=\"%d\" name=\"limit\" type=\"number\"/>"
		"</label>",
		limit
	);
}

void print_page_element(int page) {
	printf(
		"<label>Page: "
		"<input value=\"%d\" name=\"page\" type=\"number\"/>"
		"</label>",
		page
	);
}

void print_table(sqlite3* database, int limit, int page) {
	int result;
	sqlite3_stmt* query;

	result = sqlite3_prepare_v2(database, "SELECT id, name, rating FROM anime LIMIT ?1 OFFSET ?2", -1, &query, NULL);

	if(result != SQLITE_OK) {
		sqlite3_finalize(query);

		return;
	}

	result = sqlite3_bind_int(query, 1, limit);

	if(result != SQLITE_OK) {
		sqlite3_finalize(query);

		return;
	}

	result = sqlite3_bind_int(query, 2, (page - 1) * limit);

	if(result != SQLITE_OK) {
		sqlite3_finalize(query);

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
