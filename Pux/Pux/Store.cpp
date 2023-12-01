#include "Store.h"
#include "./Sqlite/sqlite3.h"
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;
using namespace std;


// todo: replace this namespace with sql factory
namespace Store {
	const char* sqlLite3DBDirectory = "./StockData/SpecialEvents.db";

	vector<vector<string>> selectQuarterlyReportDates() {
		sqlite3* database;
		sqlite3_stmt* statement;
		vector<vector<string>> selectedData = {};
		
		if (sqlite3_open(sqlLite3DBDirectory, &database) == SQLITE_OK) {
			sqlite3_prepare(database, "SELECT * FROM quarterly_earnings LIMIT 10", -1, &statement, nullptr);
			sqlite3_step(statement);
			while (sqlite3_column_text(statement, 0)) {
				vector<string> row(4);
				for (int i = 0; i < 4; i++) {
					row.at(i) = reinterpret_cast<const char*>(sqlite3_column_text(statement, i));
				}
				selectedData.push_back(row);
				sqlite3_step(statement);
			}
			sqlite3_finalize(statement);
			sqlite3_close(database);
		}
		return selectedData;
	}


}