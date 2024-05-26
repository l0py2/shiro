#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <unistd.h>

#include "cgiutils.h"

#define DATABASE_PATH "/srv/db/anime.db"
#define MAIN_TEMPLATE "templates/main.html"

void handle_get(sqlite3* database);
void handle_post(sqlite3* database);

void extract_parameters(int* limit, int* page);
void extract_input(char* input, char* name, int* rating);

void print_limit_element(int limit);
void print_page_element(int page);

void print_table(sqlite3* database, int limit, int page);

int main(void) {
	sqlite3* database;
	int result;

	result = sqlite3_open(DATABASE_PATH, &database);

	if(result != SQLITE_OK) {
		sqlite3_close(database);

		server_error("Database error");

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

void handle_get(sqlite3* database) {
	FILE* template = fopen(MAIN_TEMPLATE, "r");

	if(!template) {
		server_error("Template error");

		return;
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
}

void handle_post(sqlite3* database) {
	const char* content_length_str = getenv("CONTENT_LENGTH");

	if(!content_length_str) {
		server_error("Empty request");

		return;
	}

	const int content_length = atoi(content_length_str) + 1;

	char buff[content_length];

	fgets(buff, content_length, stdin);

	const int name_size = 64;

	char name[name_size];
	int rating = 1;

	extract_input(buff, name, &rating);

	const char* script_name = getenv("SCRIPT_NAME");

	int result;
	sqlite3_stmt* query;

	result = sqlite3_prepare_v2(database, "INSERT INTO anime (name, rating) VALUES (?1, ?2)", -1, &query, NULL);

	if(result != SQLITE_OK) {
		sqlite3_finalize(query);

		server_error("1");

		return;
	}

	result = sqlite3_bind_text(query, 1, name, -1, SQLITE_STATIC);

	if(result != SQLITE_OK) {
		sqlite3_finalize(query);

		server_error("Error on query binding");

		return;
	}

	result = sqlite3_bind_int(query, 2, rating);

	if(result != SQLITE_OK) {
		sqlite3_finalize(query);

		server_error("Error on query binding");

		return;
	}

	result = sqlite3_step(query);

	if(result != SQLITE_DONE) {
		sqlite3_finalize(query);

		server_error("Error on query execution");

		return;
	}

	sqlite3_finalize(query);

	redirect(script_name);
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

void extract_input(char* input, char* name, int* rating) {
	char* token = strtok(input, "&");

	while(token) {
		sscanf(token, "name=%s", name);
		sscanf(token, "rating=%d", rating);

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
