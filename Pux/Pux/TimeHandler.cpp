#include <vector>
#include <set>
#include <ctime>
#include <string>
#include <map>
#include <iostream>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include "TimeHandler.h"

using std::vector;
using std::set;
using std::string;
using std::map;
using std::tm;
using std::time_t;

// Looks like we do have dates all the way up to Late 2024, which means we don't have to infer new dates.
// Also there is too much variation between quarters (Q1 vs. Q2 vs...).
map<int, string> indexToDay = { {0, "Sunday"}, {1, "Monday"}, {2, "Tuesday"}, {3, "Wednesday"},
								{4, "Thursday"}, {5, "Friday"}, {6, "Saturday"} };

map<int, int> monthToDaysInMonth = { {1, 31}, {2, 28}, {3, 31}, {4, 30},
										{5, 31}, {6, 30}, {7, 31}, {8, 31},
										{9, 30}, {10, 31}, {11, 30}, {12, 31} };

map<int, time_t> goodFridayDates = {
	{ 2015, TimeHandler::convertDateToUnixTimestamp(4, 3, 2015) },
	{ 2016, TimeHandler::convertDateToUnixTimestamp(3, 25, 2016) },
	{ 2017, TimeHandler::convertDateToUnixTimestamp(4, 14, 2017) },
	{ 2018, TimeHandler::convertDateToUnixTimestamp(3, 30, 2018) },
	{ 2019, TimeHandler::convertDateToUnixTimestamp(4, 19, 2019) },
	{ 2020, TimeHandler::convertDateToUnixTimestamp(4, 10, 2020) },
	{ 2021, TimeHandler::convertDateToUnixTimestamp(4, 2, 2021) },
	{ 2022, TimeHandler::convertDateToUnixTimestamp(4, 15, 2022) },
	{ 2023, TimeHandler::convertDateToUnixTimestamp(4, 7, 2023) },
	{ 2024, TimeHandler::convertDateToUnixTimestamp(3, 29, 2024) },
	{ 2025, TimeHandler::convertDateToUnixTimestamp(4, 18, 2025) },
	{ 2026, TimeHandler::convertDateToUnixTimestamp(4, 3, 2026) },
	{ 2027, TimeHandler::convertDateToUnixTimestamp(3, 26, 2027) },
};

time_t TimeHandler::convertDateToUnixTimestamp(int month, int day, int year) {
	tm timeStruct = {};
	timeStruct.tm_mon = month;
	timeStruct.tm_mday = day;
	timeStruct.tm_year = year - 1900;

	return std::mktime(&timeStruct);
}

// todo: instead of month, day, year separately, date should probably be an object
vector<int> TimeHandler::convertUnixTimestampToDate(time_t timestamp) {
	struct tm timeStruct;
	localtime_s(&timeStruct, &timestamp);
	return { timeStruct.tm_mon, timeStruct.tm_mday, timeStruct.tm_year + 1900 };
}

int TimeHandler::findDaysBetween(time_t startTimestamp, time_t endTimestamp) {
	double differenceInSeconds = std::difftime(endTimestamp, startTimestamp);
	return differenceInSeconds / (24 * 60 * 60);
}

string TimeHandler::getDayOfWeek(time_t timestamp) {
	struct tm timeStruct;
	localtime_s(&timeStruct, &timestamp);
	int dayOfWeek = timeStruct.tm_wday;
	return indexToDay[dayOfWeek];
}

bool TimeHandler::isStockExchangeOpen(time_t timestamp) {
	string day = getDayOfWeek(timestamp);
	if (day == "Saturday" || day == "Sunday") return false;

	generateHolidaySetIfEmpty(convertDateToUnixTimestamp(1, 1, 2020), convertDateToUnixTimestamp(1, 1, 2025));
	if (holidays.find(timestamp) != holidays.end()) return false;

	return true;
}

void TimeHandler::generateHolidaySetIfEmpty(time_t startTimestamp, time_t endTimestamp) {
	if (!holidays.empty()) return;
	vector<int> startDate = convertUnixTimestampToDate(startTimestamp);
	vector<int> endDate = convertUnixTimestampToDate(endTimestamp);
	int endYear = endDate[2];
	bool addToHolidays = false;

	for (int year = startDate[2]; year <= endYear ; year++) {
		vector<time_t> yearlyHolidays = {
		convertDateToUnixTimestamp(1, 1, year), // New Year's Day
		findxDayOfMonth(3, "Monday", 1, year), // Martin Luther King Jr. Day
		findxDayOfMonth(3, "Monday", 2, year), // Washington's Birthday
		goodFridayDates[year], // Good Friday
		findLastDayOfMonth("Monday", 5, year),// Memorial Day
		convertDateToUnixTimestamp(6, 19, year), // Juneteenth National Independence Day
		convertDateToUnixTimestamp(7, 4, year), // Independence Day
		findxDayOfMonth(1, "Monday", 9, year), // Labor Day
		findxDayOfMonth(4, "Thursday", 11, year), // Thanksgiving
		convertDateToUnixTimestamp(12, 25, year), // Christmas
		};
		for (time_t holiday : yearlyHolidays) {
			vector<int> date = convertUnixTimestampToDate(holiday);
			if (date[0] > endDate[0] && date[1] > endDate[1] && year == endYear) break;
			if (holiday > startTimestamp) holidays.insert(holiday);
		}
	}
}

time_t TimeHandler::findxDayOfMonth(int x, string dayOfWeek, int month, int year) {
	int startingDay;
	for (startingDay = 1; startingDay <= 7; startingDay++) {
		if (getDayOfWeek(convertDateToUnixTimestamp(month, startingDay, year)) == dayOfWeek) break;
	}
	int day = startingDay;
	for (int i = 1; i <= 5; i++) {
		if (x == i) break;
		day += 7;
	}
	return convertDateToUnixTimestamp(month, day, year);
}

time_t TimeHandler::findLastDayOfMonth(string dayOfWeek, int month, int year) {
	int startingDay;
	for (startingDay = 1; startingDay <= 7; startingDay++) {
		if (getDayOfWeek(convertDateToUnixTimestamp(month, startingDay, year)) == dayOfWeek) break;
	}

	int daysInMonth = month == 2 && year % 4 ? 29 : monthToDaysInMonth[month];
	int day = startingDay;
	for (int i = 1; i <= 6; i++) {
		if (day > daysInMonth) break;
		day += 7;
	}
	return convertDateToUnixTimestamp(month, day - 7, year);
}

TimeHandler::TimeHandler() {
	std::cout << "Class TimeHandler is not supposed to be instantiated" << std::endl;
	exit(0);
}

