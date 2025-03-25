/**
@file volatilitytermstructure.hpp
@brief Class representing a volatility term structure
 */

#ifndef QF_VOLATILITYTERMSTRUCTURE_HPP
#define QF_VOLATILITYTERMSTRUCTURE_HPP

#include <qflib/defines.hpp>
#include <qflib/exception.hpp>
#include <qflib/math/interpol/piecewisepolynomial.hpp>
#include <qflib/sptr.hpp>
#include <string>

BEGIN_NAMESPACE(qf)

/** The volatility term structure */
class VolatilityTermStructure
{
public:

  enum class VolType
  {
    SPOTVOL,
    FWDVOL
  };

  template<typename XITER, typename YITER>
  VolatilityTermStructure(XITER tMatBegin,
             XITER tMatEnd,
             YITER volBegin,
             YITER volEnd,
             VolType vtype = VolType::SPOTVOL);

  double spotVol(double tMat) const;
  double fwdVol(double tMat1, double tMat2) const;

protected:

private:
  void initFromVarRates();
  PiecewisePolynomial fwdvars_;  
};

using SPtrVolatilityTermStructure = std::shared_ptr<VolatilityTermStructure>;

template<typename XITER, typename YITER>
VolatilityTermStructure::VolatilityTermStructure(XITER tMatBegin,
                       XITER tMatEnd,
                       YITER volBegin,
                       YITER volEnd,
                       VolType vtype)
: fwdvars_(tMatBegin, tMatEnd, volBegin, 0)
{
  std::ptrdiff_t n = tMatEnd - tMatBegin;
  QF_ASSERT(n == volEnd - volBegin, "YieldCurve: different number of maturities and rates");
  auto it = std::find_if_not(tMatBegin, tMatEnd, [](double x) {return x > 0.0;});
  QF_ASSERT(it == tMatEnd, "YieldCurve: maturities must be positive");

  switch (vtype) {
  case VolatilityTermStructure::VolType::SPOTVOL:
    initFromVarRates();
    break;
  case VolatilityTermStructure::VolType::FWDVOL:
    break;
  default:
    QF_ASSERT(0, "error: unknown volatility curve input type");
  }
}
END_NAMESPACE(qf)

#endif // QF_VOLATILITYTERMSTRUCTURE_HPP