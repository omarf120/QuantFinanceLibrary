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

def koFwd(spot, strike, KOLevel, timeToExp, timeToKO, intRate, divYield, volatility):
    """Price of a knockout forward contract in the Black-Scholes model.

    Parameters
    ----------
    spot : double
        Asset spot price
    strike : double
        Strike price for the forward contract
    KOLevel : double
        Barrier (knockout) level
    timeToExp : double
        Time to expiration in years
    timeToKO : double
        Time to knockout in years (must be less than or equal to timeToExp)
    intRate : double
        Interest rate, p.a. and continuous compounding
    divYield : double
        Asset dividend yield, p.a. and continuous compounding
    volatility : double
        Asset return volatility

    Returns
    -------
    double
        Price of the knockout forward contract
    """
    return pyqflib.koFwd(spot, strike, KOLevel, timeToExp, timeToKO, intRate, divYield, volatility)

