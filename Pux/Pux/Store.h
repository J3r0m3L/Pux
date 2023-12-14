#pragma once
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;

namespace Store {
	extern const char* sqlLite3DBDirectory;

	// todo: instead of storing in vector<vector<string>> these really should be stored in objects
	vector<vector<string>> selectQuarterlyReportDates(string companyTicker);

	vector<string> selectDistinctCompanyTickers();

	vector<vector<string>> selectEntriesByCompanyTicker(string companyTicker);
}