/**
@file  simplepricers.hpp
@brief Declaration of simple pricing functions
*/

#ifndef QF_SIMPLEPRICERS_HPP
#define QF_SIMPLEPRICERS_HPP

#include <qflib/defines.hpp>
#include <qflib/exception.hpp>
#include <qflib/sptr.hpp>
#include <qflib/market/yieldcurve.hpp>


BEGIN_NAMESPACE(qf)

/** The forward price of an asset */
double fwdPrice(double spot, double timeToExp, double intRate, double divYield);

/** Price of a European digital option in the Black-Scholes model*/
double digitalOptionBS(int payoffType, double spot, double strike, double timeToExp,
                       double intRate, double divYield, double volatility);

/** Price of a European option in the Black-Scholes model*/
double europeanOptionBS(int payoffType, double spot, double strike, double timeToExp, 
                        double intRate, double divYield, double volatility);


/** Price of a caplet or floorlet using the BS model */
double capFloorletBS(int payoffType, SPtrYieldCurve spyc, double strikeRate,double timeToReset, double tenor, double fwdRateVol);

/** CDS present value calculation for both legs */
Vector cdsPV(SPtrYieldCurve sprfyc, double credSprd, double cdsRate, double recov, double timeToMat, double payFreq);
       
END_NAMESPACE(qf)

#endif // QF_SIMPLEPRICERS_HPP
