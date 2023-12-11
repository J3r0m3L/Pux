#include <iostream>
#include <vector>
#include "Store.h"
#include "Date.h"
#include "Gaussian.h"
#include "BlackScholes.h"

using std::cout;
using std::endl;
using std::vector;

int main() {

	// Lets get the number of stock market
	// For now:
	// 1. Grab new entries from database. For a specific stock.
	//		a. create function to get distinct companies
	// 2. Use dates, to find the volatility a week, month, day before quarterly report.
	//		a. Implement blackscholes and determine good strike price to settle on with volatility. 
	// 3. Lastly compare the shit.
	
	
	vector<vector<string>> quarterlyReportDates = Store::selectQuarterlyReportDates();
	//std::cout << quarterlyReportDates.size() << std::endl;
	for (int i = 0; i < quarterlyReportDates.size(); i++) {
		for (int j = 0; j < quarterlyReportDates[i].size(); j++) {
			//std::cout << quarterlyReportDates[i][j] << std::endl;
		}
	}


	vector<string> distinctCompaniesTickers = Store::selectDistinctCompanyTickers();
	for (int i = 0; i < distinctCompaniesTickers.size(); i++) {
		// std::cout << distinctCompaniesTickers[i] << std::endl;
	}
	vector<vector<string>> prices = Store::selectEntriesByCompanyTicker("FLWS");
	for (int i = 0; i < prices.size(); i++) {
		for (int j = 0; j < prices[i].size(); j++) {
			std::cout << prices[i][j] << std::endl;
		}
	}


	return 0;
}