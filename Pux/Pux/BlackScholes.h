#pragma once

class BlackScholes {
public:
	double sharePrice; // dollars
	double strikePrice; // dollars
	double interestRate; // percentage
	double volatility; // percentage
	double timeToMaturity; // years

	BlackScholes(double sharePrice, double strikePrice, double interestRate, double volatility, double timeToMaturity);

	double calclulateCallPrice();
	double calculatePutPrice();
	double callVolatility();
	double putVolatility();
};