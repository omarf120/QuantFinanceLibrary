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

def qFwdPrice(spot, timetoexp, intrate, divyield, assetvol, fxvol, assetfxcor):
    """Quanto forward price of an asset.

    Parameters
    ----------
    spot : double
        asset spot price
    timetoexp : double
        time to expiration in years
    intrate : double
        foreign interest rate, p.a. and c.c.
    divyield : double    
        asset dividend yield, p.a. and c.c.
    assetvol : double
        asset volatility, p.a. and c.c.
    fxvol : double
        FX volatility, p.a. and c.c.
    assetfxcor : double
        asset FX correlation

    Returns
    -------
    double
        asset quanto forward price
     """
    return pyqflib.qFwdPrice(spot, timetoexp, intrate, divyield, assetvol, fxvol, assetfxcor)
