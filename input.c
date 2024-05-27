#include "input.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void decode_spaces(char* str) {
	const int str_length = strlen(str);

	for(int i = 0; i < str_length; i++)
		if(str[i] == '+')
			str[i] = ' ';
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

	decode_spaces(name);
}

void extract_update_input(char* input, int* id, char* name, int* rating) {
	char* token = strtok(input, "&");

	while(token) {
		sscanf(token, "id=%d", id);
		sscanf(token, "name=%s", name);
		sscanf(token, "rating=%d", rating);

		token = strtok(NULL, "&");
	}

	decode_spaces(name);
}

void extract_delete_input(char* input, int* id) {
	char* token = strtok(input, "&");

	while(token) {
		sscanf(token, "id=%d", id);

		token = strtok(NULL, "&");
	}
}
