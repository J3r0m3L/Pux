#pragma once
#include <vector>
#include <ctime>
#include <string>
#include <map>

using std::time_t;
using std::vector;
using std::string;
using std::map;

namespace TimeHandler {
	// date example: "Oct 31, 2024"
	time_t convertDateToUnixTimestamp(string date);

	int findDaysBetweenDates(string startDate, string endDate);

	int findDaysBetweenUnixTimestamps(time_t startTimestamp, time_t endTimestamp);

	string getDayOfWeek(time_t timestamp);

	// current exchanges: XNYS, XNAS (combine all holidays)
	bool isStockExchangeOpen();
}