from scipy import signal
import matplotlib.pyplot as plt
import numpy as np

num = [1]
den = [1, 1, 0]
sys = signal.TransferFunction(num, den)
# t, y = signal.impulse(sys)
# plt.plot(t, np.squeeze(y))
t, y = signal.step(sys)
plt.plot(t, y)
plt.show()
