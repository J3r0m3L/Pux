#include <iostream>
#include <vector>
#include "Store.h"
#include "Date.h"

using std::cout;
using std::endl;
using std::vector;

int main() {
	vector<vector<string>> tmp = Store::selectQuarterlyReportDates();
	// Right now matrix is rotated 90
	/* 
	* Goal of Date Class is to find the average number of days between.
	* If not too much variation then we can find future quarterly report dates by just adding days
	* If there is too much variation then we can assume that future quarterly reports will occur the same day as last year
	*/

	for (int i = 1; i < tmp.size(); i++) {
	
		cout << tmp.at(i).at(2) << endl;
		Date(tmp.at(i).at(2));
		break;
	}
	return 0;
}