# qflib library examples

#%%
import qflib as qf
import numpy as np
import os

ver = qf.version()
print(f'qflib version: {ver}')
pid = os.getpid()
print(f'pid: {pid}')

name = "World"
print(qf.sayHello(name))

x = [1, 2, 3]
y = [4, 5]
op = qf.outerProd(x, y)
print(f'x: {x}\ny: {y}')
print(f'outerProd:\n{op}')

#%%
# function group 1
print('=================')
print('Analytic prices')

#fwdprice
fwdpx = qf.fwdPrice(spot = 100, timetoexp = 1.0, intrate = 0.02, divyield = 0.04)
print(f'FwdPrice={fwdpx:.4f}')

qfwdpx = qf.qFwdPrice(spot = 100, timetoexp = 1.0, intrate = 0.02, divyield = 0.04, assetvol = 0.2, fxvol = 0.3, assetfxcor = 0.5)
print(f'qFwdPrice={qfwdpx:.4f}')

print(qf.__file__)  # This should now return a valid file path


# %%
