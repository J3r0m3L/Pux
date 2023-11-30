#pragma once
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;

namespace Store {
	extern const char* sqlLite3DBDirectory;

	vector<vector<string>> selectQuarterlyReportDates();
}