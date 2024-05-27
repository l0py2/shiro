#include "cgiutils.h"

#include <stdio.h>
#include <stdlib.h>

void server_error(const char* message) {
	printf("Status: 500\n");
	printf("Content-Type: text/plain\n\n");
	printf("%s\n", message);
}

void redirect(const char* path) {
	const char* query_str = getenv("QUERY_STRING");

	if(query_str)
		printf("Location: %s?%s\n\n", path, query_str);
	else
		printf("Location: %s\n\n", path);
}

void redirect_itself() {
	const char* script_name = getenv("SCRIPT_NAME");
	redirect(script_name);
}
