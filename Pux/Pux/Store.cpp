#include "Store.h"
#include "./Sqlite/sqlite3.h"
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;
using namespace std;


// todo: replace this namespace with sql factory
// bad: keep on redeclaring, opening, and closing sqlite3 database
namespace Store {
	const char* sqlLite3DBDirectory = "./StockData/SpecialEvents.db";

	vector<vector<string>> selectQuarterlyReportDates(string companyTicker) {
		sqlite3* database;
		sqlite3_stmt* statement;
		vector<vector<string>> selectedData = {};
		
		if (sqlite3_open(sqlLite3DBDirectory, &database) == SQLITE_OK) {
			sqlite3_prepare_v2(database, "SELECT * FROM quarterly_earnings WHERE company_ticker = ?", -1, &statement, nullptr);
			sqlite3_bind_text(statement, 1, companyTicker.c_str(), -1, SQLITE_STATIC);
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

	vector<string> selectDistinctCompanyTickers() {
		sqlite3* database;
		sqlite3_stmt* statement;
		vector<string> selectedData = {};

		if (sqlite3_open(sqlLite3DBDirectory, &database) == SQLITE_OK) {
			sqlite3_prepare(database, "SELECT DISTINCT company_ticker FROM quarterly_earnings", -1, &statement, nullptr);
			sqlite3_step(statement);
			while (sqlite3_column_text(statement, 0)) {
				selectedData.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
				sqlite3_step(statement);
			}
			sqlite3_finalize(statement);
			sqlite3_close(database);
		}
		return selectedData;
	}

	vector<vector<string>> selectEntriesByCompanyTicker(string companyTicker) {
		sqlite3* database;
		sqlite3_stmt* statement;
		vector<vector<string>> selectedData = {};
		if (sqlite3_open(sqlLite3DBDirectory, &database) == SQLITE_OK) {
			sqlite3_prepare_v2(database, "SELECT * FROM aggregates WHERE company_ticker = ?", -1, &statement, nullptr);
			sqlite3_bind_text(statement, 1, companyTicker.c_str(), -1, SQLITE_STATIC);
			sqlite3_step(statement);
			while (sqlite3_column_text(statement, 0)) {
				vector<string> row(9);
				for (int i = 0; i < 9; i++) {
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