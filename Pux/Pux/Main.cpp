#include <iostream>
#include <vector>
#include "Store.h"


using std::cout;
using std::endl;
using std::vector;

int main() {
	vector<vector<string>> tmp = Store::selectQuarterlyReportDates();
	// Right now matrix is rotated 90
	for (int i = 0; i < tmp.size(); i++) {
		for (int j = 0; j < tmp.at(i).size(); j++) {
			cout << tmp.at(i).at(j) << ", ";
		}
		cout << endl;
	}
	return 0;
}