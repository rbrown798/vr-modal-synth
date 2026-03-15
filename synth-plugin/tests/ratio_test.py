"""

    Seeing if we can predict the ratios we don't know for marimba
    
"""

import matplotlib.pyplot as plt
import numpy as np

overtones = [3.92, 9.24, 16.27, 24.22, 33.54, 42.97]
x = np.arange(0, 6, 1)

plt.scatter(x, overtones)
plt.plot(overtones)
plt.show()

o = overtones
print( o[3] + 0.5 * (o[5] - o[3]), o[4] )

overtones.append( o[5] + (o[5] - o[4]) )
print(o[-1])
x = np.arange(0, len(o), 1)
plt.scatter(x, o)
plt.plot(o)
plt.show()
