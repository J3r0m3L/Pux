#pragma once
#include <vector>
#include <ctime>
#include <string>
#include <map>

using std::vector;
using std::string;
using std::map;

class Date {
private:
	static bool compareDates(const Date& date1, const Date& date2);

public:
	int month, day, year;
	string strDate;
	tm tmDate;

	// date example: "Oct 31, 2024"
	Date(const string &date);

	static int FindDaysBetweenDates(Date startDate, Date endDate);

	static int FindAverageDaysBetweenDates(const vector<Date> &dates);
};