import serial
import numpy as np
import creador

ser = serial.Serial('/dev/ttyUSB0')

leidos = 0
termino = 0

def leer():
    global leidos
    global termino

    termino = 0
    matriz = np.full((37,180), 20.00)
    fila = 36
    columna = 179
    movimientoPositivo = 0

    dato = 1

    while (dato != '@@@@@'):
        while(dato != '######'):

            # Leo un dato 
            dato = ser.read(ser.in_waiting)
            while(dato == ''):
                dato = ser.read(ser.in_waiting)

            # Si es un numero
            if((dato != '@@@@@') and (dato != '#####')):
                # Lo guardo en la matriz
                matriz[fila][columna] = float(dato)
                leidos = leidos + 1

                # Lo visualizo
                print(fila),
                print(columna),
                print(dato)

                # Modifico el valor de la columna
                if((movimientoPositivo == 1) and (columna < 179)):
                    columna = columna + 1
                if((movimientoPositivo == 0) and (columna > 0)):
                    columna = columna - 1
            else:
                break

        fila = fila - 1

        # Cambio el sentido del eje x
        if(columna == 179):
            movimientoPositivo = 0
        else:
            movimientoPositivo = 1
    
    print("Se va a crear la imagen")
    creador.crear_imagen(matriz)
    leidos = 0
    termino = 1

def obtenerMatriz():
    return matriz

def enviar(comando):
    ser.write(comando)

def getPorcentaje():
    global leidos
    global termino
    porc = (leidos / (37.00 * 180.00)) * 100.00
    if(termino == 1):
        termino = 0
        return -1
    else:
        return porc