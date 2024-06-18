#include "handles.h"

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgiutils.h"
#include "input.h"
#include "elements.h"
#include "database.h"

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

	char input[content_length];

	fgets(input, content_length, stdin);

	if(strstr(input, "action=save")) {
		handle_update(database, input);
	}else if(strstr(input, "action=delete")) {
		handle_delete(database, input);
	}else {
		handle_addition(database, input);
	}
}

void handle_addition(sqlite3* database, char* input) {
	char name[name_size + 1];
	int rating = 1;

	extract_input(input, name, &rating);

	int result;
	sqlite3_stmt* query;

	result = sqlite3_prepare_v2(database, "INSERT INTO anime (name, rating) VALUES (?1, ?2)", -1, &query, NULL);

	if(result != SQLITE_OK) {
		sqlite3_finalize(query);

		server_error("Error on query creation");

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

	redirect_itself();
}

void handle_update(sqlite3* database, char* input) {
	int id;
	char name[name_size + 1];
	int rating;

	extract_update_input(input, &id, name, &rating);

	int result;
	sqlite3_stmt* query;

	result = sqlite3_prepare_v2(database, "UPDATE anime SET name=?1, rating=?2 WHERE id = ?3", -1, &query, NULL);

	if(result != SQLITE_OK) {
		sqlite3_finalize(query);

		server_error("Error on query creation");

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

	result = sqlite3_bind_int(query, 3, id);

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

	redirect_itself();
}

void handle_delete(sqlite3* database, char* input) {
	int id;

	extract_delete_input(input, &id);

	int result;
	sqlite3_stmt* query;

	result = sqlite3_prepare_v2(database, "DELETE FROM anime WHERE id = ?1", -1, &query, NULL);

	if(result != SQLITE_OK) {
		sqlite3_finalize(query);

		server_error("Error on query creation");

		return;
	}

	result = sqlite3_bind_int(query, 1, id);

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

	redirect_itself();
}
