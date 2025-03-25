/**
@file  volatilitytermstructure.cpp
@brief Implementation of the VolatilityTermStructure class
*/

#include <qflib/market/volatilitytermstructure.hpp>

BEGIN_NAMESPACE(qf)

using namespace std;

void VolatilityTermStructure::initFromVarRates()
{
  auto cit = fwdvars_.coeff_begin(0);

  double T1 = fwdvars_.breakPoint(0);
  double vol1 = *cit;
  fwdvars_.setBreakPoint(0, 0.0); 
  ++cit;
  for (size_t i = 1; i < fwdvars_.size(); ++i, ++cit) {
    double T2 = fwdvars_.breakPoint(i);
    double vol2 = *cit;
    double fvar = (pow(vol2, 2) * T2 - pow(vol1, 2) * T1)/(T2 - T1);
    QF_ASSERT(fvar >= 0.0,
      "YieldCurve: negative fwd var between T1 = " + to_string(T1) +" and T2 = " + to_string(T2));
    fwdvars_.setBreakPoint(i, T1);
    *cit = fvar;
    T1 = T2;
    vol1 = vol2;
  }
}

double VolatilityTermStructure::spotVol(double tMat) const
{
  QF_ASSERT(tMat >= 0.0, "VolCurve: spot vols for negative times not allowed");
  double integratedVar = fwdvars_.integral(0.0, tMat);
  return sqrt(integratedVar / tMat);
}
double VolatilityTermStructure::fwdVol(double tMat1, double tMat2) const
{
    QF_ASSERT(tMat1 >= 0.0, "VolatilityTermStructure: negative starting time not allowed");
    QF_ASSERT(tMat1 < tMat2, "VolatilityTermStructure: maturities must be strictly increasing");
    double integratedFwdVar = fwdvars_.integral(tMat1, tMat2);
    return sqrt(integratedFwdVar / (tMat2 - tMat1));
}

END_NAMESPACE(qf)
