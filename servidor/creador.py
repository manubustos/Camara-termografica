import matplotlib.pyplot as plt
import numpy as np
import os

def crear_imagen(m):
    plt.clf()
    plt.imshow(m, cmap='hot',interpolation='nearest', aspect='auto')
    #plt.imshow(m, cmap='hot',interpolation='bilinear', aspect='auto')
    plt.colorbar()
    plt.savefig("static/images/imagen.png")
