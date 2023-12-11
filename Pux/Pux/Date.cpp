#include <vector>
#include <ctime>
#include <string>
#include <map>
#include <iostream>
#include <chrono>
#include <sstream>
#include <algorithm>
#include "Date.h"

using std::vector;
using std::string;
using std::map;
using std::chrono::system_clock;
using std::tm;
using namespace std;
using namespace std::chrono;

// Looks like we do have dates all the way up to Late 2024, which means we don't have to infer new dates.
// Also there is too much variation between quarters (Q1 vs. Q2 vs...).
map<string, int> monthAbbreviationToInteger = { {"Jan", 0}, {"Feb", 1}, {"Mar", 2}, {"Apr", 3},
												{"May", 4}, {"Jun", 5}, {"Jul", 6}, {"Aug", 7},
												{"Sep", 8}, {"Oct", 9}, {"Nov", 10}, {"Dec", 11} };


int month, day, year = 0;
string strDate;
tm tmDate;


Date::Date(const string &date) {
	strDate = date;
	month = monthAbbreviationToInteger[date.substr(0, 3)], day = stoi(date.substr(3, 3)), year = stoi(date.substr(7, 5));
	tmDate = { 0, 0, 0, day, month, year - 1900 }; // should eventually include time
}

// todo: figure out if possible to use reference to parameter instead of copy
int Date::FindDaysBetweenDates(Date startDate, Date endDate) {
	time_t start = mktime(&startDate.tmDate);
	time_t end = mktime(&endDate.tmDate);
	return abs(end - start) / 86400;
}

// todo: figure out if possible to use reference to parameter instead of copy
int Date::FindAverageDaysBetweenDates(const vector<Date> &dates) {
	vector<Date> orderedDates = dates;
	sort(orderedDates.begin(), orderedDates.end(), compareDates);
	
	int sum = 0;
	for (int i = 1; i < dates.size(); i++) {
		sum += FindDaysBetweenDates(orderedDates.at(i - 1), orderedDates.at(i));
	}
	return sum / dates.size() - 1;
}

// should eventually adjust for time
bool Date::compareDates(const Date& date1, const Date& date2) {
	tm tm1 = date1.tmDate, tm2 = date2.tmDate;
	if (tm1.tm_year != tm2.tm_year) {
		return tm1.tm_year < tm2.tm_year;
	}
	else if (tm1.tm_mon != tm2.tm_mon) {
		return tm1.tm_mon < tm2.tm_mon;
	}
	else if (tm1.tm_mday != tm2.tm_mday) {
		return tm1.tm_mday < tm2.tm_mday;
	}
	return false;
};