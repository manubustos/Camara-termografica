import matplotlib.pyplot as plt
import numpy as np
import os
from html_variables import * 
import lector_datos

def crear_html_imagen(): # Crea el html con la imagen escaneada.
    filas = 36
    columnas = 179
    m = lector_datos.crearMatriz()

    with open("templates/resultado.html", "w") as file:
        file.write('<!DOCTYPE html>{0}<body style="background: none;">'.format(HTML_HEAD))
        
    print ("Generando Imagen")
    plt.imshow(m, cmap='hot',interpolation='nearest', aspect='auto') #Create image with scale colors
    plt.colorbar() #Draw color bar
    os.remove("static/images/imagen.png") # Elimina la imagen anterior
    plt.savefig('static/images/imagen.png') # Sobreescribe las imagenes

    # En el archivo de texto crea divs con tooltips que indiquen la temperatura de ese punto
    html_text = ""
    for i in range (0,filas):
        for j in range (0,columnas):
            html_text += "<div class='tooltip' style=' border: none; position: absolute; width: 6px; \
height: 11px; left: {1}px; top:{0}px;'><span class='tooltiptext'>{2}&degC</span></div>\n"\
.format(i*10.27+66,j*2.23+88,m[i][j])

    with open("templates/resultado.html", "a") as file:
        file.write(html_text)

    # Pone la imagen sobre los divs
    with open("templates/resultado.html", "a") as file:
        file.write('{0}</body></html>'.format(HTML_IMAGEN))

    print ("Imagen generada")


