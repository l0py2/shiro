#include "cgiutils.h"

#include <stdio.h>

void server_error(const char* message) {
	printf("Status: 500\n");
	printf("Content-Type: text/plain\n\n");
	printf("%s\n", message);
}

void redirect(const char* path) {
	printf("Location: %s\n\n", path);
}
