#include "BlackScholes.h"
#include "Gaussian.h"
#include <cmath>

double sharePrice; // dollars
double strikePrice; // dollars
double interestRate; // percentage
double volatility; // percentage
double timeToMaturity; // years
double optionPrice; // dollars

// todo: either look into templates to generate constructor or overload it in the future
BlackScholes::BlackScholes(double sharePrice, double strikePrice, double interestRate, double timeToMaturity) {
	this->sharePrice = sharePrice;
	this->strikePrice = strikePrice;
	this->interestRate = interestRate;
	this->timeToMaturity = timeToMaturity;
}


double BlackScholes::calclulateCallPrice() {
	double d1 = log(sharePrice / strikePrice) + ((interestRate + pow(volatility, 2) / 2) * timeToMaturity) / (volatility * sqrt(timeToMaturity));
	double d2 = d1 - sqrt(volatility);
	this->optionPrice = Gaussian::cdf(d1) * sharePrice - Gaussian::cdf(d2) * strikePrice * exp(-interestRate * timeToMaturity);
	return optionPrice;
}

double BlackScholes::calculatePutPrice() {
	double d1 = log(sharePrice / strikePrice) + ((interestRate + pow(volatility, 2) / 2) * timeToMaturity) / (volatility * sqrt(timeToMaturity));
	double d2 = d1 - sqrt(volatility);
	this->optionPrice = Gaussian::cdf(-d2) * strikePrice * exp(-interestRate * timeToMaturity) - sharePrice * Gaussian::cdf(-d1);
	return optionPrice;
}


double BlackScholes::calculateCallVolatility() {
	double numerator = 2 * log(sharePrice / strikePrice) + 2 * interestRate * timeToMaturity;
	double denominator = interestRate;
	this->volatility = sqrt(numerator / denominator);
	return volatility;
}

double BlackScholes::calculatePutVolatility() {
	double numerator = 2 * log(strikePrice / sharePrice) + 2 * interestRate * timeToMaturity;
	double denominator = interestRate;
	this->volatility = sqrt(numerator / denominator);
	return volatility;
}