/**
@file  simplepricers.cpp
@brief Implementation of simple pricing functions
*/

#include <qflib/pricers/simplepricers.hpp>
#include <qflib/math/stats/normaldistribution.hpp>

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
double digitalOptionBS(int payoffType, double spot, double strike, double timeToExp,
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
  NormalDistribution normal;

  double price = exp(-intRate * timeToExp) * normal.cdf(phi * d2);

  return price;
}

/** Price of a European option in the Black-Scholes model*/
double europeanOptionBS(int payoffType, double spot, double strike, double timeToExp, 
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

  return price;
}

/** Price of a single-point knockout forward contract in the Black-Scholes model */
double knockoutFwd(double spot, double strike, double KOLevel, double timeToExp, 
  double timeToKO, double intRate, double divYield, double volatility)
{
QF_ASSERT(spot >= 0.0, "spot must be non-negative");
QF_ASSERT(strike >= 0.0, "strike must be non-negative");
QF_ASSERT(KOLevel >= 0.0, "KOLevel must be non-negative");
QF_ASSERT(timeToExp >= 0.0, "time to expiration must be non-negative");
QF_ASSERT(timeToKO >= 0.0 && timeToKO <= timeToExp, "time to KO must be within [0, timeToExp]");
QF_ASSERT(intRate >= 0.0, "interest rate must be non-negative");
QF_ASSERT(divYield >= 0.0, "dividend yield must be non-negative");
QF_ASSERT(volatility >= 0.0, "volatility must be non-negative");

double call_price = europeanOptionBS(1, spot, KOLevel, timeToKO, intRate, divYield, volatility);

double digital_call = digitalOptionBS(1, spot, KOLevel, timeToKO, intRate, divYield, volatility);

double fwd_T1 = fwdPrice(spot, timeToExp - timeToKO, intRate, divYield) * exp(-intRate * timeToKO);

return (fwd_T1 - strike) * digital_call + call_price * exp(-(intRate - divYield) * (timeToExp - timeToKO));
}

END_NAMESPACE(qf)
