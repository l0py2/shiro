#ifndef CGIUTILS_H
#define CGIUTILS_H

void server_error(const char* message);
void redirect(const char* path);
void redirect_itself();

#endif
