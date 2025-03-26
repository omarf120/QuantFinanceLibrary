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

/** Price of a caplet or floorlet using the Black-Scholes model */
double capFloorletBS(int payoffType,
  SPtrYieldCurve spyc,
  double strikeRate, 
  double timeToReset,
  double tenor,
  double fwdRateVol)
{
// Validate input parameters
QF_ASSERT(payoffType == 1 || payoffType == -1, "payoffType must be 1 (cap) or -1 (floor)");
QF_ASSERT(strikeRate > 0.0, "strikeRate must be positive");
QF_ASSERT(timeToReset >= 0.0, "timeToReset must be non-negative");
QF_ASSERT(tenor > 0.0, "tenor must be positive");
QF_ASSERT(fwdRateVol > 0.0, "fwdRateVol must be positive");


double paymentTime = timeToReset + tenor;
double df = spyc->discount(paymentTime);  
double fwdRate = spyc->fwdRate(timeToReset, paymentTime);
double fwdRateSimple = (exp(fwdRate * tenor) - 1.0) / tenor;
double phi = payoffType;
double sigma_sqrt_t = fwdRateVol * sqrt(timeToReset);
double d1 = (log(fwdRateSimple / strikeRate) + 0.5 * fwdRateVol * fwdRateVol * timeToReset) / sigma_sqrt_t;
double d2 = d1 - sigma_sqrt_t;

NormalDistribution normal;
double N_d1 = normal.cdf(phi * d1);
double N_d2 = normal.cdf(phi * d2);
double price = phi * df * tenor * (fwdRateSimple * N_d1 - strikeRate * N_d2);

return price;
}

Vector cdsPV(SPtrYieldCurve sprfyc, double credSprd, double cdsRate,
  double recov, double timeToMat, double payFreq) {

QF_ASSERT(sprfyc, "Yield curve pointer is null");
QF_ASSERT(credSprd >= 0, "Credit spread must be non-negative");
QF_ASSERT(cdsRate >= 0, "CDS rate must be non-negative");
QF_ASSERT(recov >= 0 && recov <= 1, "Recovery rate must be between 0 and 1");
QF_ASSERT(timeToMat > 0, "Time to maturity must be positive");
QF_ASSERT(payFreq > 0, "Payment frequency must be positive");

int numPayments = std::ceil(timeToMat * payFreq);

Vector result(2);
double defaultPV = 0.0;
double premiumPV = 0.0;

std::vector<double> paymentTimes;
for (int i = 1; i <= numPayments; ++i) {
double paymentTime = i / payFreq;
if (paymentTime > timeToMat) {
 paymentTime = timeToMat;
}
paymentTimes.push_back(paymentTime);
}

std::vector<double> survivalProbs;
survivalProbs.push_back(1.0); 

for (size_t i = 0; i < paymentTimes.size(); ++i) {
double survivalProb = std::exp(-credSprd * paymentTimes[i]);
survivalProbs.push_back(survivalProb);
}

for (size_t i = 0; i < paymentTimes.size(); ++i) {
double prevSurvivalProb = survivalProbs[i];
double currSurvivalProb = survivalProbs[i + 1];
double defaultProb = prevSurvivalProb - currSurvivalProb;

double discountFactor = sprfyc->discount(paymentTimes[i]);

defaultPV += (1.0 - recov) * defaultProb * discountFactor;
}

double prevPaymentTime = 0.0;

for (size_t i = 0; i < paymentTimes.size(); ++i) {
double paymentTime = paymentTimes[i];
double periodLength = paymentTime - prevPaymentTime;

double survivalProb = survivalProbs[i + 1];
double discountFactor = sprfyc->discount(paymentTime);

premiumPV += cdsRate * survivalProb * discountFactor * periodLength;

prevPaymentTime = paymentTime;
}

result(0) = defaultPV;
result(1) = premiumPV;

return result;
}

END_NAMESPACE(qf)