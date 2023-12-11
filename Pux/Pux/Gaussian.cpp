#include "Gaussian.h"
#include <cmath>

# define M_PI           3.14159265358979323846

namespace Gaussian {
	double cdf(double generator) {
		if (generator < -8.0) return 0.0;
		if (generator > 8.0) return 0.0;
		double sum = 0.0;
		double term = generator;
		int i = 3;
		while (sum + term != sum) {
			sum += term;
			term = term * pow(generator, 2) / i;
			i += 2;
		}
		return 0.5 + sum * pdf(generator);
	}

	double pdf(double generator) {
		return exp(-generator * generator / 2) / sqrt(2 * M_PI);
	}
}