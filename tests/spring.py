from scipy import signal
import matplotlib.pyplot as plt
import numpy as np

m = 1 
k = 1
zeta = 0.5
c = 2 * zeta * np.sqrt(m * k) 

num = [ 1 ]
den = [ m, c, k ]

tf = signal.TransferFunction(num, den)
t, y = signal.step(tf)

plt.plot(t, y)
plt.xlabel('Time [s]')
plt.ylabel('Position')
plt.grid()
plt.show()

plt.plot(t, y)
