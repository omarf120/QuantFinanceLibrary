import qflib.pyqflib

###################
# function group 0

def version(): 
    """qflib library version.

    Returns
    -------
    str
        version in the format Major.Minor.Revision
    """
    return pyqflib.version()


def sayHello(name): 
    """Says hello.

    Parameters
    ----------
    name : str
        caller name

    Returns
    -------
    str
        greeting
    """
    return pyqflib.sayHello(name)


def outerProd(vector1, vector2):
    """Computes the outer product of two numerical vectors.

    Parameters
    ----------
    vector1 : list(double) or 1D numpy array
        first vector
    vector2: list(double) or 1D numpy array
        second vector

    Returns
    -------
    2D numpy array
        outer product as matrix
    """
    return pyqflib.outerProd(vector1, vector2)


def erf(x):
    """Error function at `x`. 
    """
    return pyqflib.erf(x)


def invErf(x):
    """Inverse error function at `x`. 
    """
    return pyqflib.invErf(x)


def normalCdf(x):
    """Cumulative standard normal distribution at `x`. 
    """
    return pyqflib.normalCdf(x)


def normalInvCdf(x):
    """Inverse of the cumulative standard normal distribution at quantile `x`.
    """
    return pyqflib.normalInvCdf(x)


def echoMatrix(mat):
    """Echoes the input matrix.

    Parameters
    ----------
    mat : 2D numpy array
        input matrix

    Returns
    -------
    2D numpy array
        the input matrix `mat`
    """
    return pyqflib.echoMatrix(mat)


def ppolyEval(bkpoints, values, polyorder, xvec, derivorder): 
    """Values and derivatives of a piecewise polynomial function at a set of points.

    Parameters
    ----------
    bkpoints : list(double) or 1D numpy array
        breakpoints of the piecewise polynomial function
    values : list(double) or 1D numpy array
        corresponding values at each breakpoint
    polyorder : {0, 1}
        polynomial order of the segments (constant or linear)
    xvec : list(double) or 1D numpy array
        points at which values or derivatives are evaluated
    derivorder : int (>= 0)
        if 0, values are computed, else derivatives of `derivorder` order  

    Returns
    -------
    1D numpy array
        values or derivatives at each point in `xvec`
    
    Notes
    -----
    1. If `polyorder`==0, the value at the left breakpoint is used for the segment.
    2. At points outside the breakpoint span constant extrapolation is used.
    """
    return pyqflib.ppolyEval(bkpoints, values, polyorder, xvec, derivorder)

###################
# function group 1

def fwdPrice(spot, timetoexp, intrate, divyield):
    """Forward price of an asset.

    Parameters
    ----------
    spot : double
        asset spot price
    timetoexp : double
        time to expiration in years
    intrate : double
        interest rate, p.a. and c.c.
    divyield : double    
        asset dividend yield, p.a. and c.c.

    Returns
    -------
    double
        asset forward price
     """
    return pyqflib.fwdPrice(spot, timetoexp, intrate, divyield)


def digiBS(payofftype, spot, strike, timetoexp, intrate, divyield, volatility):
    """Price of a European digital option in the Black-Scholes model. 

    Parameters
    ----------
    payofftype : {1, -1}
        1 for call, -1 for put
    spot : double
        asset spot price
    strike : double
        strike price
    timetoexp : double
        time to expiration in years
    intrate : double
        interest rate, p.a. and c.c.
    divyield : double    
        asset dividend yield, p.a. and c.c.
    volatility : double
        asset return volatility

    Returns
    -------
    dictionary
        price of the option
    """
    return pyqflib.digiBS(payofftype, spot, strike, timetoexp, intrate, divyield, volatility)


def euroBS(payofftype, spot, strike, timetoexp, intrate, divyield, volatility):
    """Price and Greeks of a European option in the Black-Scholes model. 

    Parameters
    ----------
    payofftype : {1, -1}
        1 for call, -1 for put
    spot : double
        asset spot price
    strike : double
        strike price
    timetoexp : double
        time to expiration in years
    intrate : double
        interest rate, p.a. and c.c.
    divyield : double    
        asset dividend yield, p.a. and c.c.
    volatility : double
        asset return volatility

    Returns
    -------
        price of the option
    """
    return pyqflib.euroBS(payofftype, spot, strike, timetoexp, intrate, divyield, volatility)


def ppolyIntegral(bkpoints, values, polyorder, xstart, xvec): 
    """Integral of a piecewise polynomial function from `xstart` to each point in `xvec`.

    Parameters
    ----------
    bkpoints : list(double) or 1D numpy array
        brekpoints of the piecewise polynomial function
    values : list(double) or 1D numpy array
        corresponding values at each breakpoint
    polyorder : {0, 1}
        polynomial order of the segments (constant or linear)
    xstart : double
        starting point of each interval
    xvec : list(double) or 1D numpy array
        end points one for each interval

    Returns
    -------
    1D numpy array
        values of the integral
    
    Notes
    -----
    1. If `polyorder`==0, the value at the left breakpoint is used for the segment.
    2. At points outside the breakpoint span constant extrapolation is used.
    """
    return pyqflib.ppolyIntegral(bkpoints, values, polyorder, xstart, xvec)

###################
# function group 2

def mktList():
    """Names of all market objects.

    Returns
    -------
    dictionary
        YieldCurves : list with names of yield curves
        Volatilities : list with names of volatility term structures   
    """
    return pyqflib.mktList()


def mktClear():
    """Clears all market objects.

    Returns
    -------
    TRUE
    """
    return pyqflib.mktClear()


def ycCreate(ycname, tmats, vals, valtype):
    """Creates a new yield curve.

    Parameters
    ----------
    ycname : str
        new yield curve name
    tmats : list(double) or 1D numpy array
        maturities in years
    vals : list(double) or 1D numpy array
        zero bond prices or interest rates to each maturity in `tmats`
    valtype : {0, 1, 2}
        0: zero bond prices, 1: spot interest rates, 2: forward interest rates

    Returns
    -------
    str 
        name of the newly created yield curve

    Notes
    -----
    1. Yield curve names are case insensitive.
    2. A new yield curve replaces an existing one if they have the same name.
    """
    return pyqflib.ycCreate(ycname, tmats, vals, valtype)


def discount(ycname, tmat):
    """Discount factor from yield curve.

    Parameters
    ----------
    ycname : str
        name of the yield curve
    tmat : double
        time to maturity of the discount factor, in years

    Returns
    -------
    double
        discount factor
    """
    return pyqflib.discount(ycname, tmat)


def fwdDiscount(ycname, tmat1, tmat2):
    """Forward discount factor from yield curve.

    Parameters
    ----------
    ycname : str
        name of the yield curve
    tmat1 : double
        time to reset of the forward discount factor, in years
    tmat2 : double
        time to maturity of the forward discount factor, in years

    Returns
    -------
    double
        forward discount factor
    """
    return pyqflib.fwdDiscount(ycname, tmat1, tmat2)


def spotRate(ycname, tmat):
    """Spot interest rate from yield curve.

    Parameters
    ----------
    ycname : str
        name of the yield curve
    tmat : double
        time to maturity of the interest rate, in years

    Returns
    -------
    double
        spot interest rate
    """
    return pyqflib.spotRate(ycname, tmat)


def fwdRate(ycname, tmat1, tmat2):
    """Forward interest rate from yield curve.

    Parameters
    ----------
    ycname : str
        name of the yield curve
    tmat1 : double
        time to reset of the forward discount rate, in years
    tmat2 : double
        time to maturity of the forward discount rate, in years

    Returns
    -------
    double
        the forward discount rate
    """
    return pyqflib.fwdRate(ycname, tmat1, tmat2)

def volCreate(volname, tmats, vols, voltype):
    """Creates a new vol curve.

    Parameters
    ----------
    volname : str
        new vol curve name
    tmats : list(double) or 1D numpy array
        maturities in years
    vols : list(double) or 1D numpy array
        zero bond prices or interest rates to each maturity in `tmats`
    voltype : {0, 1}
        0: zero bond prices, 1: spot interest rates, 2: forward interest rates

    Returns
    -------
    str 
        name of the newly created yield curve

    Notes
    -----
    1. vol curve names are case insensitive.
    2. A new vol curve replaces an existing one if they have the same name.
    """
    return pyqflib.volCreate(volname, tmats, vols, voltype)

def spotVol(volname, tmat):
    """Spot interest rate from yield curve.

    Parameters
    ----------
    vcname : str
        name of the yield curve
    tmat : double
        time to maturity of the interest rate, in years

    Returns
    -------
    double
        spot interest rate
    """
    return pyqflib.spotVol(volname, tmat)


def fwdVol(volname, tmat1, tmat2):
    """Forward interest rate from yield curve.

    Parameters
    ----------
    vcname : str
        name of the yield curve
    tmat1 : double
        time to reset of the forward discount rate, in years
    tmat2 : double
        time to maturity of the forward discount rate, in years

    Returns
    -------
    double
        the forward discount rate
    """
    return pyqflib.fwdVol(volname, tmat1, tmat2)