/** 
@file  pyfunctions1.hpp
@brief Implementation of Python callable functions
*/
#include "pycpp.hpp"
#include <qflib/defines.hpp>
#include <qflib/pricers/simplepricers.hpp>
#include <string>

static 
PyObject*  pyQfqFwdPrice(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pySpot(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyIntRate(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyAssetVol(NULL);
  PyObject* pyFXVol(NULL);
  PyObject* pyAssetFXCor(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOOOOO", &pySpot, &pyTimeToExp, &pyIntRate, &pyDivYield, &pyAssetVol, &pyFXVol, &pyAssetFXCor))
    return NULL;

  double spot = asDouble(pySpot);
  double timeToExp = asDouble(pyTimeToExp);
  double intRate = asDouble(pyIntRate);
  double divYield = asDouble(pyDivYield);
  double assetVol = asDouble(pyAssetVol);
  double fxVol = asDouble(pyFXVol);
  double assetFXCor = asDouble(pyAssetFXCor);

  double qFwd = qf::quantoFwdPrice(spot, timeToExp, intRate, divYield, assetVol, fxVol, assetFXCor);

  return asPyScalar(qFwd);
PY_END;
}

static
PyObject*  pyQfFwdPrice(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pySpot(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyIntRate(NULL);
  PyObject* pyDivYield(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pySpot, &pyTimeToExp, &pyIntRate, &pyDivYield))
    return NULL;

  double spot = asDouble(pySpot);
  double timeToExp = asDouble(pyTimeToExp);
  double intRate = asDouble(pyIntRate);
  double divYield = asDouble(pyDivYield);

  double fwd = qf::fwdPrice(spot, timeToExp, intRate, divYield);

  return asPyScalar(fwd);
PY_END;
}
