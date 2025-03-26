/** 
@file  pyfunctions0.hpp
@brief Implementation of Python callable functions
*/
#include <pyqflib/pyutils.hpp>
#include <qflib/defines.hpp>
#include <qflib/math/stats/errorfunction.hpp>
#include <qflib/math/stats/normaldistribution.hpp>
#include <qflib/math/interpol/piecewisepolynomial.hpp>
#include <qflib/math/optim/polyfunc.hpp>
#include <qflib/math/optim/roots.hpp>
#include <qflib/utils.hpp>
#include <string>

static 
PyObject*  pyQfVersion(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  return asPyScalar(std::string(QF_VERSION_STRING));
PY_END;
}

static 
PyObject* pyQfSayHello(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyArg1(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyArg1))
    return NULL;
  std::string name = asString(pyArg1);        // read in the name
  std::string greet = "Hello " + name + "!";  // compose the greeting  
  return asPyScalar(greet);
PY_END;
}

static 
PyObject* pyQfOuterProd(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyArg1(NULL);
  PyObject* pyArg2(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OO", &pyArg1, &pyArg2))
    return NULL;

  std::vector<double> vec1 = asDblVec(pyArg1);
  std::vector<double> vec2 = asDblVec(pyArg2);
  size_t nrow = vec1.size();
  size_t ncol = vec2.size();
  
  // allocate and compute the answer
  std::vector<std::vector<double>> outprod(nrow);
  for (size_t i = 0; i < nrow; ++i) {
	  outprod[i].resize(ncol);
	  for (size_t j = 0; j < ncol; ++j) {
		  outprod[i][j] = vec1[i] * vec2[j];
	  };
  };
	
  return asPyArray(outprod);

PY_END;
}

static
PyObject*  pyQfErf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  return asPyScalar(qf::ErrorFunction::erf(x));
PY_END;
}

static
PyObject*  pyQfInvErf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  return asPyScalar(qf::ErrorFunction::inverf(x));
PY_END;
}

static
PyObject*  pyQfNormalCdf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  qf::NormalDistribution f;

  return asPyScalar(f.cdf(x));
PY_END;
}

static
PyObject*  pyQfNormalInvCdf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  qf::NormalDistribution f;

  return asPyScalar(f.invcdf(x));
PY_END;
}

static 
PyObject*  pyQfEchoMatrix(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyMat(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyMat))
    return NULL;

  qf::Matrix mat = asMatrix(pyMat);
  return asNumpy(mat);
PY_END;
}

static 
PyObject*  pyQfPPolyEval(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyBkPoints(NULL);
  PyObject* pyValues(NULL);
  PyObject* pyPolyOrder(NULL);
  PyObject* pyXVec(NULL);
  PyObject* pyDerivOrder(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOOO", &pyBkPoints, &pyValues, &pyPolyOrder, &pyXVec, &pyDerivOrder))
    return NULL;

  qf::Vector bkpts = asVector(pyBkPoints);
  qf::Vector vals = asVector(pyValues);
  int degree = asInt(pyPolyOrder);
  qf::Vector xx = asVector(pyXVec);
  QF_ASSERT(bkpts.size() == vals.size(), "unequal number of breakpoints and vals");
  int derivOrder = asInt(pyDerivOrder);

  // create the curve
  qf::PiecewisePolynomial pp(bkpts.begin(), bkpts.end(), vals.begin(), degree);

  qf::Vector yy(xx.size());
  pp.eval(xx.begin(), xx.end(), yy.begin(), derivOrder);

  return asNumpy(yy);
PY_END;
}

static 
PyObject*  pyQfPPolyIntegral(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyBkPoints(NULL);
  PyObject* pyValues(NULL);
  PyObject* pyPolyOrder(NULL);
  PyObject* pyXStart(NULL);
  PyObject* pyXVecEnd(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOOO", &pyBkPoints, &pyValues, &pyPolyOrder, &pyXStart, &pyXVecEnd))
    return NULL;

  qf::Vector bkpts = asVector(pyBkPoints);
  qf::Vector vals = asVector(pyValues);
  QF_ASSERT(bkpts.size() == vals.size(), "unequal number of breakpoints and vals");
  int degree = asInt(pyPolyOrder);
  double x0 = asDouble(pyXStart);
  qf::Vector xx = asVector(pyXVecEnd);
  // create the curve
  qf::PiecewisePolynomial pp(bkpts.begin(), bkpts.end(), vals.begin(), degree);
  // integrate
  qf::Vector yy(xx.size());
  pp.integral(x0, xx.begin(), xx.end(), yy.begin());

  return asNumpy(yy);
PY_END;
}

static
PyObject*  pyQfPolyBracket(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyCoeffs(NULL);
  PyObject* pyLowerLimit(NULL);
  PyObject* pyUpperLimit(NULL);
  PyObject* pyNSubs(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pyCoeffs, &pyLowerLimit, &pyUpperLimit, &pyNSubs))
    return NULL;

  qf::Vector coeffs = asVector(pyCoeffs);
  double xlo = asDouble(pyLowerLimit);
  double xhi = asDouble(pyUpperLimit);
  int nsubs = asInt(pyNSubs);

  qf::Polynomial p(coeffs);
  qf::Vector xb1, xb2;
  int nroot;
  qf::zbrak(p, xlo, xhi, nsubs, xb1, xb2, nroot);

  qf::Matrix bkts(nroot, 2);
  for (int i = 0; i < nroot; ++i) {
    bkts(i, 0) = xb1[i];
    bkts(i, 1) = xb2[i];
  };
  return asNumpy(bkts);
PY_END;
}

static
PyObject*  pyQfPolySecant(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyCoeffs(NULL);
  PyObject* pyLowerLimit(NULL);
  PyObject* pyUpperLimit(NULL);
  PyObject* pyTol(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pyCoeffs, &pyLowerLimit, &pyUpperLimit, &pyTol))
    return NULL;

  qf::Vector coeffs = asVector(pyCoeffs);
  double xlo = asDouble(pyLowerLimit);
  double xhi = asDouble(pyUpperLimit);
  double tol = asDouble(pyTol);

  qf::Polynomial p(coeffs);
  double root = qf::rtsec(p, xlo, xhi, tol);
  return asPyScalar(root);
PY_END;
}

static
PyObject*  pyQfToContCmpd(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyRate(NULL);
  PyObject* pyAnnFreq(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OO", &pyRate, &pyAnnFreq))
    return NULL;

  double ret = qf::toContCmpd(asDouble(pyRate), asLong(pyAnnFreq));
  return asPyScalar(ret);
PY_END;
}

static
PyObject*  pyQfFromContCmpd(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyRate(NULL);
  PyObject* pyAnnFreq(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OO", &pyRate, &pyAnnFreq))
    return NULL;

  double ret = qf::fromContCmpd(asDouble(pyRate), asLong(pyAnnFreq));
  return asPyScalar(ret);
PY_END;
}
