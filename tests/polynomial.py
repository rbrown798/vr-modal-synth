from scipy.interpolate import lagrange
import numpy as np
from numpy.polynomial.polynomial import Polynomial
import matplotlib.pyplot as plt

# f2 x and y
x = [ 0, 0.5, 1 ] 
y = [ 2.756, 3.0, 4.0 ] 

x_new = np.linspace(0, 1, 100)

coef = np.polyfit(x, y, 3)
a, b, c, d = coef
def cubic(x):
    return a*x**3 + b*x**2 + c*x + d

y_new = cubic(x_new)
plt.scatter(x, y, label='data')
plt.plot(x_new, y_new, label='Polynomial')
plt.show()


glockenspiel = [2.756, 5.404, 8.933, 13.34, 18.63, 24.81, 31.86] 
xylophone = [3.0, 6.16, 10.29, 14.01, 19.66, 24.02]
vibraphone = [4.0, 10.08, 20.0, 27.0]
#marimba = [3.92, 9.24, 16.27, 24.22, 33.54, 42.97]
marimba = [4.0, 9.24, 16.27, 24.22, 33.54, 42.97]

coefs = []
for i in range(6):
    y = [ glockenspiel[i], xylophone[i], marimba[i] ]
    c = np.polyfit(x, y, 3)
    coefs.append(c)

    # y_new = cubic(x_new)
    # plt.scatter(x, y, label='data')
    # plt.plot(x_new, y_new, label='Polynomial')
    # plt.show()

for a, b, c, d in coefs:
    print(f'f{i+2}: {a:.5} x^3 + {b:.5} x^2 + {c:.5} x + {d:.5}')

for a, b, c, d in coefs:
    print(f'f{i+2}: {a:.10} {b:.10} {c:.10} {d:.10}')
