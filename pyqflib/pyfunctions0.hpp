/** 
@file  pyfunctions0.hpp
@brief Implementation of Python callable functions
*/
#include <pyqflib/pyutils.hpp>
#include <qflib/defines.hpp>
#include <qflib/math/stats/errorfunction.hpp>
#include <qflib/math/stats/normaldistribution.hpp>
#include <qflib/math/interpol/piecewisepolynomial.hpp>
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
PyObject*  pyQfPPolySum(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyBkPoints1(NULL);
  PyObject* pyValues1(NULL);
  PyObject* pyBkPoints2(NULL);
  PyObject* pyValues2(NULL);
  PyObject* pyPOrder(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOOO", &pyBkPoints1, &pyValues1, &pyBkPoints2, &pyValues2, &pyPOrder))
    return NULL;

  qf::Vector bkpts1 = asVector(pyBkPoints1);
  qf::Vector vals1 = asVector(pyValues1);
  qf::Vector bkpts2 = asVector(pyBkPoints2);
  qf::Vector vals2 = asVector(pyValues2);
  int porder = asInt(pyPOrder);

  QF_ASSERT(porder >=0 && porder <= 1, "porder must be 0 or 1");
  QF_ASSERT(bkpts1.size() == vals1.size(), "unequal number of breakpoints1 and vals1");
  QF_ASSERT(bkpts2.size() == vals2.size(), "unequal number of breakpoints2 and vals2");

  qf::PiecewisePolynomial pp1(bkpts1.begin(), bkpts1.end(), vals1.begin(), porder);
  qf::PiecewisePolynomial pp2(bkpts2.begin(), bkpts2.end(), vals2.begin(), porder);
  
  qf::PiecewisePolynomial pp_sum = pp1 + pp2;

  qf::Vector common_bkpts = pp_sum.breakPoints();
  qf::Vector values;

  values.set_size(common_bkpts.size());  

  for (size_t i = 0; i < common_bkpts.size(); ++i) {
      values(i) = pp_sum.eval(common_bkpts[i]);  
  }

  qf::Matrix result(common_bkpts.size(), 2);

  for (size_t i = 0; i < common_bkpts.size(); ++i) {
    result(i, 0) = common_bkpts[i]; 
    result(i, 1) = values[i];        
  }

return asNumpy(result);
PY_END;
}


