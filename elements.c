#include "elements.h"

#include <sqlite3.h>
#include <stdio.h>

void print_limit_element(int limit) {
	printf(
		"<label>Limit: "
		"<input value=\"%d\" name=\"limit\" type=\"number\" min=\"1\"/>"
		"</label>",
		limit
	);
}

void print_page_element(int page) {
	printf(
		"<label>Page: "
		"<input value=\"%d\" name=\"page\" type=\"number\" min=\"1\"/>"
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

	while(sqlite3_step(query) == SQLITE_ROW)
		printf(
			"<tr>"
			"<td><input value=\"%d\" name=\"id\" form=\"form-%d\" readonly/></td>"
			"<td><input value=\"%s\" name=\"name\" form=\"form-%d\" required/></td>"
			"<td><input value=\"%d\" name=\"rating\" type=\"number\" form=\"form-%d\" min=\"1\" max=\"10\" required/></td>"
			"<td>"
			"<form method=\"post\" id=\"form-%d\">"
			"<select name=\"action\"><option value=\"save\" selected>Save</option><option value=\"delete\">Delete</option></select>"
			"<button type=\"submit\">Run</button>"
			"</form>"
			"</td>"
			"</tr>",
			sqlite3_column_int(query, 0), sqlite3_column_int(query, 0),
			sqlite3_column_text(query, 1), sqlite3_column_int(query, 0),
			sqlite3_column_int(query, 2), sqlite3_column_int(query, 0),
			sqlite3_column_int(query, 0)
		);

	sqlite3_finalize(query);
}
