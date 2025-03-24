/**
@file  simplepricers.hpp
@brief Declaration of simple pricing functions
*/

#ifndef QF_SIMPLEPRICERS_HPP
#define QF_SIMPLEPRICERS_HPP

#include <qflib/defines.hpp>
#include <qflib/exception.hpp>


BEGIN_NAMESPACE(qf)

/** The forward price of an asset */
double fwdPrice(double spot, double timeToExp, double intRate, double divYield);

/** The forward price of an asset in a GBP */
double quantoFwdPrice(double spot, double timeToExp, double intRate, double divYield, double assetVol, double fxVol, double assetFxCor);

END_NAMESPACE(qf)

#endif // QF_SIMPLEPRICERS_HPP
