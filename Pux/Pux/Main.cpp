#include <iostream>
#include <vector>
#include <ctime>
#include <set>
#include "Store.h"
#include "TimeHandler.h"
#include "Gaussian.h"
#include "BlackScholes.h"

using std::cout;
using std::endl;
using std::vector;
using std::time_t;
using std::stoi;
using std::set;

// returns vector = { volatility difference, price difference }
vector<double> volatilityAndPriceDifference(double sharePrice1, double strikePrice1, double sharePrice2, double strikePrice2, double interestRate, double timeToMaturity) {
	BlackScholes blackScholes1 = BlackScholes(sharePrice1, strikePrice1, 0.04, 0.083);
	double volatility1 = blackScholes1.calculateCallVolatility();
	double price1 = blackScholes1.calclulateCallPrice();

	BlackScholes blackScholes2 = BlackScholes(sharePrice2, strikePrice2, 0.04, 0.083);
	double volatility2 = blackScholes2.calculateCallVolatility();
	double price2 = blackScholes2.calclulateCallPrice();

	return { volatility1 - volatility2, price1 - price2 };
}

int main() {
	// Compare volatility 7 days before and at Quarterly Report for "FLWS" and print differnce

	// Convert to time_t
	vector<vector<string>> quarterlyReports = Store::selectQuarterlyReportDates("FLWS");
	vector<time_t> quarterlyReportDates(quarterlyReports.size());
	// set<time_t> quarterlyReportSet;
	for (int i = 0; i < quarterlyReports.size(); i++) {
		char lastQuarterlyReportCharacter = quarterlyReports[i][2][quarterlyReports[i][2].length() - 1];
		if (lastQuarterlyReportCharacter == ',') {
			quarterlyReports[i][2].pop_back();
		}
		quarterlyReportDates[i] = TimeHandler::convertDateToUnixTimestamp(
			TimeHandler::convertMonthToIndex[quarterlyReports[i][2].substr(0, 3)],
			stoi(quarterlyReports[i][2].substr(4, 2)),
			stoi(quarterlyReports[i][2].substr(8, 4))
		);
		// quarterlyReportSet.insert(quarterlyReportDates[i]);
	}

	// Find the dates 7 days before quarterly report
	vector<time_t> sevenDaysBeforeQuarterlyReportDates (quarterlyReportDates.size());
	// set<time_t> sevenDaysBeforeQuarterlyReportSet;
	for (int i = 0; i < quarterlyReportDates.size(); i++) {
		sevenDaysBeforeQuarterlyReportDates[i] = TimeHandler::getTimestampxTradingDaysBeforeTimestamp(7, quarterlyReportDates[i]);
		// sevenDaysBeforeQuarterlyReportSet.insert(sevenDaysBeforeQuarterlyReportDates[i]);
	}

	// Get company prices for dates above
	vector<vector<string>> prices = Store::selectEntriesByCompanyTicker("FLWS");
	vector<double> openQuarterlyReport(prices.size());
	vector<double> highQuarterlyReport(prices.size());
	vector<double> lowQuarterlyReport(prices.size());
	vector<double> closeQuarterlyReport(prices.size());

	vector<double> openBeforeQuarterlyReport(prices.size());
	vector<double> highBeforeQuarterlyReport(prices.size());
	vector<double> lowBeforeQuarterlyReport(prices.size());
	vector<double> closeBeforeQuarterlyReport(prices.size());

	// todo: this should probably be placed within the sqlQuery
	for (int i = 0; i < prices.size(); i++) {
		// j = 1 open
		// j = 2 high
		// j = 3 low
		// j = 4 close
		// j = 7 timestamp (milliseconds)

		time_t priceTimestamp = static_cast<time_t> (std::stol(prices[i][7].substr(0, prices[i][7].size() - 3)));

		for (int j = 0; j < quarterlyReportDates.size(); j++) {
			// todo: compareTimestamps needs to be renamed
			if (TimeHandler::compareTimestamps(priceTimestamp, quarterlyReportDates[j])) {
				openQuarterlyReport[j] = std::stod(prices[i][1]);
				highQuarterlyReport[j] = std::stod(prices[i][2]);
				lowQuarterlyReport[j] = std::stod(prices[i][3]);
				closeQuarterlyReport[j] = std::stod(prices[i][4]);
			}

			if (TimeHandler::compareTimestamps(priceTimestamp, sevenDaysBeforeQuarterlyReportDates[j])) {
				openBeforeQuarterlyReport[j] = std::stod(prices[i][1]);
				highBeforeQuarterlyReport[j] = std::stod(prices[i][2]);
				lowBeforeQuarterlyReport[j] = std::stod(prices[i][3]);
				closeBeforeQuarterlyReport[j] = std::stod(prices[i][4]);
			}
		}
	}

	// Interest Rate: ~4.00% Right Now
	// TimeToMaturity: 1/12 year = ~0.083 Year
	// BlackScholes Model
	map<time_t, double> openVolatilityMap;
	map<time_t, double> highVolatilityMap;
	map<time_t, double> lowVolatilityMap;
	map<time_t, double> closeVolatilityMap;

	map<time_t, double> openPriceMap;
	map<time_t, double> highPriceMap;
	map<time_t, double> lowPriceMap;
	map<time_t, double> closePriceMap;
	for (int i = 0; i < quarterlyReportDates.size(); i++) {
		if (openQuarterlyReport[i] != 0 && openBeforeQuarterlyReport[i] != 0) {
			vector<double>differences = volatilityAndPriceDifference(
				openQuarterlyReport[i],
				openQuarterlyReport[i],
				openBeforeQuarterlyReport[i],
				openBeforeQuarterlyReport[i],
				0.04,
				0.083
			);
			openVolatilityMap[quarterlyReportDates[i]] = differences[0];
			openPriceMap[quarterlyReportDates[i]] = differences[1];
		}

		if (highQuarterlyReport[i] != 0 && highBeforeQuarterlyReport[i] != 0) {
			vector<double>differences = volatilityAndPriceDifference(
				highQuarterlyReport[i],
				highQuarterlyReport[i],
				highBeforeQuarterlyReport[i],
				highBeforeQuarterlyReport[i],
				0.04,
				0.083
			);
			highVolatilityMap[quarterlyReportDates[i]] = differences[0];
			highPriceMap[quarterlyReportDates[i]] = differences[1];
		}

		if (lowQuarterlyReport[i] != 0 && lowBeforeQuarterlyReport[i] != 0) {
			vector<double>differences = volatilityAndPriceDifference(
				lowQuarterlyReport[i],
				lowQuarterlyReport[i],
				lowBeforeQuarterlyReport[i],
				lowBeforeQuarterlyReport[i],
				0.04,
				0.083
			);
			lowVolatilityMap[quarterlyReportDates[i]] = differences[0];
			lowPriceMap[quarterlyReportDates[i]] = differences[1];
		}

		if (closeQuarterlyReport[i] != 0 && closeBeforeQuarterlyReport[i] != 0) {
			vector<double>differences = volatilityAndPriceDifference(
				closeQuarterlyReport[i],
				closeQuarterlyReport[i],
				closeBeforeQuarterlyReport[i],
				closeBeforeQuarterlyReport[i],
				0.04,
				0.083
			);
			closeVolatilityMap[quarterlyReportDates[i]] = differences[0];
			closePriceMap[quarterlyReportDates[i]] = differences[1];
		}
	}

	// print out results
	int openVolatilitySum = 0;
	int highVolatilitySum = 0;
	int lowVolatilitySum = 0;
	int closeVolatilitySum = 0;
	int openPriceSum = 0;
	int highPriceSum = 0;
	int lowPriceSum = 0;
	int closePriceSum = 0;

	for (std::map<time_t, double>::iterator iter = openVolatilityMap.begin(); iter != openVolatilityMap.end(); ++iter) {
		time_t timestamp = iter->first;
		vector<int> date = TimeHandler::convertUnixTimestampToDate(timestamp);
		for (int i = 0; i < date.size(); i++) {
			std::cout << date[i] << " ";
		}
		std::cout << std::endl;

		std::cout << "Open Volatility: " << openVolatilityMap[timestamp] << std::endl;
		std::cout << "High Volatility: " << highVolatilityMap[timestamp] << std::endl;
		std::cout << "Low Volatility: " << lowVolatilityMap[timestamp] << std::endl;
		std::cout << "Close Volatility: " << closeVolatilityMap[timestamp] << std::endl;
		std::cout << "Open Price: " << openPriceMap[timestamp] << std::endl;
		std::cout << "High Price: " << highPriceMap[timestamp] << std::endl;
		std::cout << "Low Price: " << lowPriceMap[timestamp] << std::endl;
		std::cout << "Close Price: " << closePriceMap[timestamp] << std::endl;

		openVolatilitySum += openVolatilityMap[timestamp];
		highVolatilitySum += highVolatilityMap[timestamp];
		lowVolatilitySum += lowVolatilityMap[timestamp];
		closeVolatilitySum += closeVolatilityMap[timestamp];
		openPriceSum += openPriceMap[timestamp];
		highPriceSum += highPriceMap[timestamp];
		lowPriceSum += lowPriceMap[timestamp];
		closePriceSum += closePriceMap[timestamp];
	}

	// print out average for all of them
	std::cout << "Averages: " << std::endl;
	std::cout << "Open Volatility Avg: " << openVolatilitySum / openVolatilityMap.size() << std::endl;
	std::cout << "High Volatility Avg: " << highVolatilitySum / highVolatilityMap.size() << std::endl;
	std::cout << "Low Volatility Avg: " << lowVolatilitySum / lowVolatilityMap.size() << std::endl;
	std::cout << "Close Volatility Avg: " << closeVolatilitySum / closeVolatilityMap.size() << std::endl;
	std::cout << "Open Price Avg: " << openPriceSum / openPriceMap.size() << std::endl;
	std::cout << "High Price Avg: " << highPriceSum / highPriceMap.size() << std::endl;
	std::cout << "Low Price Avg: " << lowPriceSum / lowPriceMap.size() << std::endl;
	std::cout << "Close Price Avg: " << closePriceSum / closePriceMap.size() << std::endl;
}

int test() {
	vector<vector<string>> quarterlyReports = Store::selectQuarterlyReportDates("FLWS");
	vector<time_t> quarterlyReportDates (quarterlyReports.size());
	for (int i = 0; i < quarterlyReports.size(); i++) {
		char lastQuarterlyReportCharacter = quarterlyReports[i][2][quarterlyReports[i][2].length() - 1];
		if (lastQuarterlyReportCharacter == ',') {
			quarterlyReports[i][2].pop_back();
		}
		if (i >= 5) continue;
		quarterlyReportDates[i] = TimeHandler::convertDateToUnixTimestamp(
									TimeHandler::convertMonthToIndex[quarterlyReports[i][2].substr(0, 3)],
									stoi(quarterlyReports[i][2].substr(4, 2)),
									stoi(quarterlyReports[i][2].substr(8, 4))
								  );
		if (i < 5) {
			// std::cout << 11 << std::endl;
			std::cout << TimeHandler::convertMonthToIndex[quarterlyReports[i][2].substr(0, 3)] << " ";
			std::cout << stoi(quarterlyReports[i][2].substr(4, 2)) << " ";
			std::cout << stoi(quarterlyReports[i][2].substr(8, 4)) << std::endl;
		}
	}
	std::cout << "Successfully Stored" << std::endl;
	for (int i = 0; i < 5; i++) {
		vector<int> firstDate = TimeHandler::convertUnixTimestampToDate(quarterlyReportDates[i]);
		for (int i = 0; i < firstDate.size(); i++)
			std::cout << firstDate[i] << " ";
		std::cout << std::endl;
	}
	std::cout << "Successfully Retreived" << std::endl;
	for (int i = 0; i < 5; i++) {
		int daysBefore = 7;
		time_t dateBefore = TimeHandler::getTimestampxTradingDaysBeforeTimestamp(daysBefore, quarterlyReportDates[i]);
		vector<int> firstDate = TimeHandler::convertUnixTimestampToDate(dateBefore);
		for (int i = 0; i < firstDate.size(); i++)
			std::cout << firstDate[i] << " ";
		std::cout << std::endl;
	}
	std::cout << "Successfully Calculated Date Before" << std::endl;



	



	//std::cout << quarterlyReportDates.size() << std::endl;
	//for (int i = 0; i < quarterlyReportDates.size(); i++) {

	//	std::cout << quarterlyReportDates[i][2] << std::endl;


		// for (int j = 0; j < quarterlyReportDates[i].size(); j++) {
		//	std::cout << quarterlyReportDates[i][j] << std::endl;
		// }
	//}

	/*
	vector<string> distinctCompaniesTickers = Store::selectDistinctCompanyTickers();
	for (int i = 0; i < distinctCompaniesTickers.size(); i++) {
		// std::cout << distinctCompaniesTickers[i] << std::endl;
	}
	vector<vector<string>> prices = Store::selectEntriesByCompanyTicker("FLWS");
	for (int i = 0; i < prices.size(); i++) {
		for (int j = 0; j < prices[i].size(); j++) {
			// std::cout << prices[i][j] << std::endl;
		}
	}
	*/

	return 0;
}