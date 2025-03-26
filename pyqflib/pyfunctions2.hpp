/**
@file  pyfunctions2.hpp
@brief Implementation of Python callable functions
*/
#include <pyqflib/pyutils.hpp>
#include <qflib/pricers/simplepricers.hpp>
#include <qflib/defines.hpp>
#include <qflib/market/market.hpp>

static
PyObject*  pyQfMktList(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  std::vector<std::string> ycnames = qf::market().yieldCurves().list();

  // return market contents as a Python dictionary
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("YieldCurves"), asPyList(ycnames));
  return ret;
PY_END;
}

static
PyObject*  pyQfMktClear(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  qf::market().clear();
  return asPyScalar(true);
PY_END;
}

static
PyObject*  pyQfYCCreate(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyYCName(NULL);
  PyObject* pyTMats(NULL);
  PyObject* pyVals(NULL);
  PyObject* pyValType(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pyYCName, &pyTMats, &pyVals, &pyValType))
    return NULL;

  std::string name = asString(pyYCName);
  qf::Vector tmats = asVector(pyTMats);
  qf::Vector vals = asVector(pyVals);

  int valtype = asInt(pyValType);
  qf::YieldCurve::InputType intype;
  switch (valtype) {
  case 0:
    intype = qf::YieldCurve::InputType::SPOTRATE;
    break;
  case 1:
    intype = qf::YieldCurve::InputType::FWDRATE;
    break;
  case 2:
    intype = qf::YieldCurve::InputType::ZEROBOND;
    break;
  default:
    QF_ASSERT(0, "error: unknown yield curve input type");
  }

  std::pair<std::string, unsigned long> pr =
    qf::market().yieldCurves().set(name,
      std::make_shared<qf::YieldCurve>(tmats.begin(), tmats.end(), vals.begin(), vals.end(), intype)
    );

  std::string tag = pr.first;
  return asPyScalar(tag);
PY_END;
}

static
PyObject*  pyQfDiscount(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyCrvName(NULL);
  PyObject* pyMat(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OO", &pyCrvName, &pyMat))
    return NULL;

  std::string name = asString(pyCrvName);
  double tmat = asDouble(pyMat);

  qf::SPtrYieldCurve spyc = qf::market().yieldCurves().get(name);
  QF_ASSERT(spyc, "error: yield curve " + name + " not found");

  double df = spyc->discount(tmat);
  return asPyScalar(df);
PY_END;
}

static
PyObject*  pyQfFwdDiscount(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyCrvName(NULL);
  PyObject* pyMat1(NULL);
  PyObject* pyMat2(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOO", &pyCrvName, &pyMat1, &pyMat2))
    return NULL;

  std::string name = asString(pyCrvName);
  double T1 = asDouble(pyMat1);
  double T2 = asDouble(pyMat2);

  qf::SPtrYieldCurve spyc = qf::market().yieldCurves().get(name);
  QF_ASSERT(spyc, "error: yield curve " + name + " not found");

  double fdf = spyc->fwdDiscount(T1, T2);
  return asPyScalar(fdf);
PY_END;
}

static
PyObject*  pyQfSpotRate(PyObject* pyDummy, PyObject* pyArgs) 
{
PY_BEGIN;

  PyObject* pyCrvName(NULL);
  PyObject* pyMat(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OO", &pyCrvName, &pyMat))
    return NULL;

  std::string name = asString(pyCrvName);
  double tmat = asDouble(pyMat);

  qf::SPtrYieldCurve spyc = qf::market().yieldCurves().get(name);
  QF_ASSERT(spyc, "error: yield curve " + name + " not found");

  double srate = spyc->spotRate(tmat);
  return asPyScalar(srate);
PY_END;
}

static
PyObject*  pyQfFwdRate(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyCrvName(NULL);
  PyObject* pyMat1(NULL);
  PyObject* pyMat2(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOO", &pyCrvName, &pyMat1, &pyMat2))
    return NULL;

  std::string name = asString(pyCrvName);
  double T1 = asDouble(pyMat1);
  double T2 = asDouble(pyMat2);

  qf::SPtrYieldCurve spyc = qf::market().yieldCurves().get(name);
  QF_ASSERT(spyc, "error: yield curve " + name + " not found");

  double frate = spyc->fwdRate(T1, T2);
  return asPyScalar(frate);
PY_END;
}


static
PyObject* pyQfCapFloorletBS(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyPayType(NULL);
  PyObject* pyYCName(NULL);
  PyObject* pyStrikeRate(NULL);
  PyObject* pyTimeToReset(NULL);
  PyObject* pyTenor(NULL);
  PyObject* pyFwdRateVol(NULL);
  
  if (!PyArg_ParseTuple(pyArgs, "OOOOOO", &pyPayType, &pyYCName, &pyStrikeRate, 
                        &pyTimeToReset, &pyTenor, &pyFwdRateVol))
    return NULL;
    
  int payType = asInt(pyPayType);
  std::string ycName = asString(pyYCName);
  double strikeRate = asDouble(pyStrikeRate);
  double timeToReset = asDouble(pyTimeToReset);
  double tenor = asDouble(pyTenor);
  double fwdRateVol = asDouble(pyFwdRateVol);
  
  // Get yield curve from market
  qf::SPtrYieldCurve spyc = qf::market().yieldCurves().get(ycName);
  QF_ASSERT(spyc, "error: yield curve " + ycName + " not found");
  
  // Call the C++ function
  double price = qf::capFloorletBS(payType, spyc, strikeRate, timeToReset, tenor, fwdRateVol);
  
  return asPyScalar(price);
PY_END;
}

static
PyObject* pyQfCdsPV(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyRfreeYC(NULL);
  PyObject* pyCredSpread(NULL);
  PyObject* pyCdsRate(NULL);
  PyObject* pyRecov(NULL);
  PyObject* pyTimeToMat(NULL);
  PyObject* pyPayFreq(NULL);
  
  if (!PyArg_ParseTuple(pyArgs, "OOOOOO", &pyRfreeYC, &pyCredSpread, &pyCdsRate, 
                        &pyRecov, &pyTimeToMat, &pyPayFreq))
    return NULL;
    
  std::string rfreeYC = asString(pyRfreeYC);
  double credSpread = asDouble(pyCredSpread);
  double cdsRate = asDouble(pyCdsRate);
  double recov = asDouble(pyRecov);
  double timeToMat = asDouble(pyTimeToMat);
  double payFreq = asDouble(pyPayFreq);
  
  // Get yield curve from market
  qf::SPtrYieldCurve sprfyc = qf::market().yieldCurves().get(rfreeYC);
  QF_ASSERT(sprfyc, "error: yield curve " + rfreeYC + " not found");
  
  // Call the C++ function
  qf::Vector result = qf::cdsPV(sprfyc, credSpread, cdsRate, recov, timeToMat, payFreq);
  
  // Convert result to NumPy array
  return asNumpy(result);
PY_END;
}