#pragma once
#include <vector>
#include <ctime>
#include <string>
#include <map>
#include <set>

using std::time_t;
using std::vector;
using std::set;
using std::string;
using std::map;

class TimeHandler {
public:
	static map<string, int> convertMonthToIndex;

	static time_t convertDateToUnixTimestamp(int month, int day, int year);

	static vector<int> convertUnixTimestampToDate(time_t timeStamp);

	static int findDaysBetween(time_t startTimestamp, time_t endTimestamp);

	static string getDayOfWeek(time_t timestamp);

	static bool isStockExchangeOpen(time_t timestamp);

	static time_t getTimestampxTradingDaysBeforeTimestamp(int days, time_t timestamp);

	static bool compareTimestamps(const time_t timestamp1, const time_t timestamp2);
private:
	static void generateHolidaySetIfEmpty(time_t startTimestamp, time_t endTimestamp);

	static time_t findxDayOfMonth(int x, string dayOfWeek, int month, int year);

	static time_t findLastDayOfMonth(string dayOfWeek, int month, int year);

	static set<time_t> holidays;

	TimeHandler();
};