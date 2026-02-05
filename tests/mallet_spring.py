import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

fs = 44100

density = 1100
diameter_cm = 20
springConstant = 2.81e+08
dampingConstant = 100 #3076.0

radius = diameter_cm / 100
volume = (4/3) * np.pi * radius ** 3
mass = density * volume
dt = 1 / fs


a = mass / (dt ** 2) + dampingConstant / (2 * dt)
b = springConstant - mass / (dt ** 2) * 2
c = mass / (dt ** 2) - dampingConstant / (2 * dt)

b1 = 1 / a
a1 = b / a
a2 = c / a

print(b1, a1, a2)

tf = signal.TransferFunction([1, b1], [1, a1, a2], dt=dt)

w, mag, phase = signal.dbode(tf, n=100)
plt.semilogx(w, mag)
plt.show()

t, y = signal.dimpulse(tf, n=100)
plt.plot(t, np.squeeze(y))
plt.show()
        
omega = np.sqrt(springConstant / mass)
#r = 0.99
r = np.exp( -(dampingConstant / (2 * mass)) / fs)

theta = omega / fs
a1 = -2 * r * np.cos(theta)
a2 = r ** 2
b0 = 1

tf = signal.TransferFunction([b0], [1, a1, a2], dt=dt)
w, mag, phase = signal.dbode(tf, n=100)
plt.semilogx(w, mag)
plt.show()
t, y = signal.dimpulse(tf, n=100)
plt.plot(t, np.squeeze(y))
plt.show()

