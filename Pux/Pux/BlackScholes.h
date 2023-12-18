#pragma once

class BlackScholes {
public:
	double sharePrice; // dollars
	double strikePrice; // dollars
	double interestRate; // percentage
	double volatility; // percentage
	double timeToMaturity; // years
	double optionPrice; // dollars

	BlackScholes(double sharePrice, double strikePrice, double interestRate, double timeToMaturity);

	double calclulateCallPrice();
	double calculatePutPrice();
	double calculateCallVolatility();
	double calculatePutVolatility();
};