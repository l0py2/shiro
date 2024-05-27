#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <sqlite3.h>

void print_limit_element(int limit);
void print_page_element(int page);
void print_table(sqlite3* database, int limit, int page);

#endif
