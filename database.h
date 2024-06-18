#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

static const int name_size = 128;

int create_table(sqlite3* database);

#endif
