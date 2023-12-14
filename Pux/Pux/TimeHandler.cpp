#include <vector>
#include <ctime>
#include <string>
#include <map>
#include <iostream>
#include <chrono>
#include <sstream>
#include <algorithm>
#include "TimeHandler.h"

using std::vector;
using std::string;
using std::map;
using std::tm;
using std::time_t;

// Looks like we do have dates all the way up to Late 2024, which means we don't have to infer new dates.
// Also there is too much variation between quarters (Q1 vs. Q2 vs...).
map<string, int> monthAbbreviationToInteger = { {"Jan", 0}, {"Feb", 1}, {"Mar", 2}, {"Apr", 3},
												{"May", 4}, {"Jun", 5}, {"Jul", 6}, {"Aug", 7},
												{"Sep", 8}, {"Oct", 9}, {"Nov", 10}, {"Dec", 11} };



namespace TimeHandler {
	time_t convertDateToUnixTimestamp(string date) {
		tm timeStruct = {};
		timeStruct.tm_mon = monthAbbreviationToInteger[date.substr(0, 3)];
		timeStruct.tm_mday = stoi(date.substr(3, 3));
		timeStruct.tm_year = stoi(date.substr(7, 5)) - 1900;

		// should eventually include time
		return std::mktime(&timeStruct);
	}

	int findDaysBetweenDates(string startDate, string endDate) {
		time_t startTimestamp = convertDateToUnixTimestamp(startDate);
		time_t endTimestamp = convertDateToUnixTimestamp(endDate);
		return findDaysBetweenUnixTimestamps(startTimestamp, endTimestamp);
	}

	int findDaysBetweenUnixTimestamps(time_t startTimestamp, time_t endTimestamp) {
		time_t differenceInSeconds = std::difftime(endTimestamp, startTimestamp);
		return differenceInSeconds / (24 * 60 * 60);
	}

	string getDayOfWeek(time_t timestamp) {
		tm* timeStruct = std::localtime(&timestamp);
		char dayOfWeek[20];
		std::strftime(dayOfWeek, sizeof(dayOfWeek), "%A", timeStruct);
		return dayOfWeek;
	}

	// NYSE and Nasdaq Holidays
	// New Year's Day: Monday, Jan 2
	// Martin Luther King Jr. Day: Monday, Jan 16
	// Washington's Birthday: Monday, Feb 20
	// Good Friday: Friday, April 7
	// Memorial Day: Monday, May 29
	// Juneteenth National Independence Day: Monday, June 19
	// Independence Day: Tuesday, July 4
	// Labor Day: Monday, Sept 4
	// Thanksgiving: Thursday, Nov 23
	// Christmas: Monday, Dec 25

	//bool isStockExchangeOpen() {
	//}
}