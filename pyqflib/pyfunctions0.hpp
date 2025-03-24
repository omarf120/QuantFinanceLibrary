/** 
@file  pyfunctions0.hpp
@brief Implementation of Python callable functions
*/
#include "pycpp.hpp"
#include <qflib/defines.hpp>

static
PyObject* pyQfPolyProd(PyObject* pyDummy, PyObject* pyArgs)
{
  PY_BEGIN;

  PyObject* pyArg1 = NULL;
  PyObject* pyArg2 = NULL;

  if (!PyArg_ParseTuple(pyArgs, "OO", &pyArg1, &pyArg2))
    return NULL;

  std::vector<double> p = asDblVec(pyArg1);
  std::vector<double> q = asDblVec(pyArg2);

  if (p.empty() || q.empty())
  {
    throw std::invalid_argument("Input Vectors must not be empty");
  }

  size_t result_size = p.size() + q.size() - 1;
  std::vector<double> result(result_size, 0);

  for (size_t i = 0; i < p.size(); ++i)
  {
    for (size_t j = 0; j < q.size(); ++j)
    {
      result[i + j] += p[i] * q[j];
    }
  }

  while (result.size() > 1 && result.back() == 0)
  {
    result.pop_back();
  }

  if (result.empty())
  {
    result.push_back(0);
  }

  return asPyArray(result);

  PY_END;
}

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

