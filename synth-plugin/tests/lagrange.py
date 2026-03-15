from scipy.interpolate import lagrange
import numpy as np
from numpy.polynomial.polynomial import Polynomial
import matplotlib.pyplot as plt

# f2 x and y
x = [ 0, 0.5, 1 ] 
y = [ 2.756, 3.0, 4.0 ] 

poly = lagrange(x, y)
print(Polynomial(poly.coef[::-1]).coef)

x_new = np.arange(0, 1, 0.1)
print(poly.coef)

plt.scatter(x, y, label='data')
plt.plot(x_new, Polynomial(poly.coef[::-1])(x_new), label='Polynomial')
plt.show()


from scipy.interpolate import PchipInterpolator
p = PchipInterpolator(x, y)
#a, b, c = p.coef
y_new = p(x_new)

plt.scatter(x, y, label='data')
plt.plot(x_new, y_new, label='Polynomial')
plt.show()
print(p.c)

coef = np.polyfit(x, y, 3)
a, b, c, d = coef
def cubic(x):
    return a*x**3 + b*x**2 + c*x + d
y_new = cubic(x_new)
plt.scatter(x, y, label='data')
plt.plot(x_new, y_new, label='Polynomial')
plt.show()

coef = np.polyfit(x, y, 2)
a, b, c = coef
def quadratic(x):
    return a*x**2 + b*x + c
y_new = quadratic(x_new)
plt.scatter(x, y, label='data')
plt.plot(x_new, y_new, label='Polynomial')
plt.show()
