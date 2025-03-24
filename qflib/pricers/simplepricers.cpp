/**
@file  simplepricers.cpp
@brief Implementation of simple pricing functions
*/

#include <qflib/pricers/simplepricers.hpp>
#include <qflib/math/stats/normaldistribution.hpp>
#include <qflib/math/matrix.hpp>

#include <cmath>

BEGIN_NAMESPACE(qf)

/** The forward price of an asset */
double fwdPrice(double spot, double timeToExp, double intRate, double divYield)
{
  QF_ASSERT(spot >= 0.0, "spot must be non-negative");
  QF_ASSERT(timeToExp >= 0.0, "time to expiration must be non-negative");
  QF_ASSERT(intRate >= 0.0, "interest rate must be non-negative");
  QF_ASSERT(divYield >= 0.0, "dividend yield must be non-negative");

  return spot * exp((intRate - divYield) * timeToExp);
}

/** Price of a European digital option in the Black-Scholes model*/
qf::Vector digitalOptionBS(int payoffType, double spot, double strike, double timeToExp,
  double intRate, double divYield, double volatility)
{
QF_ASSERT(payoffType == 1 || payoffType == -1, "payoffType must be 1 or -1");
QF_ASSERT(strike >= 0.0, "strike must be non-negative");
QF_ASSERT(divYield >= 0.0, "dividend yield must be non-negative");
QF_ASSERT(volatility >= 0.0, "volatility must be non-negative");

double phi = payoffType;
double fwd = fwdPrice(spot, timeToExp, intRate, divYield);
double sigT = volatility * sqrt(timeToExp);
double d2 = log(fwd / strike) / sigT - 0.5 * sigT;
double d1 = d2 + sigT;
NormalDistribution normal;

double price = exp(-intRate * timeToExp) * normal.cdf(phi * d2);
double delta = phi * (exp(-intRate * timeToExp) / (spot * sigT)) * normal.pdf(d2);
double gamma = -phi * ((exp(-intRate * timeToExp)) / (pow(spot, 2) * pow(volatility, 2) * timeToExp)) * d1 * normal.pdf(d2);
double theta = -intRate * exp(-intRate * timeToExp) * normal.cdf(phi * d2) + phi * exp(-intRate * timeToExp) * normal.pdf(d2) * (d2 / (2 * timeToExp));
double vega = phi * (exp(-intRate * timeToExp) * normal.pdf(d2) * -(d1 / volatility));

return qf::Vector{price, delta, gamma, theta, vega};
}

/** Price of a European option in the Black-Scholes model*/
qf::Vector europeanOptionBS(int payoffType, double spot, double strike, double timeToExp, 
  double intRate, double divYield, double volatility)
{
QF_ASSERT(payoffType == 1 || payoffType == -1, "payoffType must be 1 or -1");
QF_ASSERT(strike >= 0.0, "strike must be non-negative");
QF_ASSERT(volatility >= 0.0, "volatility must be non-negative");

double phi = payoffType;
double fwd = fwdPrice(spot, timeToExp, intRate, divYield);
double sigT = volatility * sqrt(timeToExp);
double d1 = log(fwd / strike) / sigT + 0.5 * sigT;
double d2 = d1 - sigT;

NormalDistribution normal;
double df = exp(-intRate * timeToExp);
double price = fwd * normal.cdf(phi * d1) - strike * normal.cdf(phi * d2);
price *= phi * df;
double delta;
if (payoffType == -1)
  delta = exp(-divYield * timeToExp) * (normal.cdf(d1) + phi);
else
  delta = exp(-divYield * timeToExp) * normal.cdf(d1);
double gamma = (exp(-divYield * timeToExp) * normal.pdf(d1)) / (spot * sigT);
double theta = (-exp(-divYield * timeToExp) * spot * volatility) / (2 * sqrt(timeToExp)) * normal.pdf(d1) + phi * divYield * exp(-divYield * timeToExp) * spot * normal.cdf(phi * d1) - phi * intRate * exp(-intRate * timeToExp) * strike * normal.cdf(phi * d2);
double vega = exp(-divYield * timeToExp) * spot * sqrt(timeToExp) * normal.pdf(d1);

return qf::Vector{price, delta, gamma, theta, vega};
}

END_NAMESPACE(qf)
