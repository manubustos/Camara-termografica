# Lee los datos del puerto serie que genera el sensor en el Arduino
# Genera una matriz con los datos

import serial
import time
import numpy as np

ser = serial.Serial('/dev/ttyUSB1')#, 9600, timeout=0)

def iniciar(): # Genera los dos archivos de texto vacios
    with open("datos.txt", "w") as file: # Todos los datos que lee del puerto serie, con o sin errores
        file.write("")
    with open("datosVerificados.txt", "w") as file: # Todos los datos verificados (sin errores)
        file.write("")
    
def leer(): # Lee los datos del puerto serie y los escribe en datos.txt
    minimo = 999
    maximo = -999

    #Leo el primer dato
    try:
        dato = ser.read(8)
        print (dato)
    except:
        print('No se pudo leer el dato')
    time.sleep(0.035)
    #time.sleep(0.040)
    #time.sleep(0.1)
    #time.sleep(0.5)

    while ("@" not in dato):
        if (dato != ""):
            try:
                dato = ser.read(8)
                print (dato)
            except:
                print('No se pudo leer el dato')

            if (dato != '#####' and "@" not in dato and dato != ""):
                #guarda el dato leido en el archivo de texto
                with open("datos.txt", "a") as file:
                    file.write("{0}\n".format(dato))
            else:
                if (dato == '#####'): # avanzo de fila
                    with open("datos.txt", "a") as file:
                        file.write("#####")

            time.sleep(0.035) #mismo tiempo con el cual lee el arduino
            #time.sleep(0.040)
            #time.sleep(0.1) #segundos
            #time.sleep(0.5)
        else: #lee un espacio en blanco
            try:
                with open("datos.txt", "a") as file:
                    file.write("{0}\n".format(dato))

                dato = ser.read(8)
                print (dato)
            except:
                print('No se pudo leer el dato')
            time.sleep(0.035)
            #time.sleep(0.040)
            #time.sleep(0.1)
            #time.sleep(0.5)

    #Salio del while por lo tanto finalizo
    if ("@" in dato):
        # Muestro la pagina con el resultado
        print ("finalizo el escaneo")
        ser.close()

def verificar(): # verifica que los valores leidos del puerto serie tengan el formato xx.xx
    print ("Verificando Datos")
    verificadorInicio = False
    pocosCaracteres = False
    with open("datos.txt", "r") as f:
        lineas = f.readlines()
    for i in range(len(lineas)):
        l=lineas[i]
        ultimoDato="" # Dato anterior
        if (verificadorInicio == False and len(l) == 1): # Elimina los primeros datos vacios
            pass
        else:
            if (verificadorInicio == False):
                skip = i
            verificadorInicio = True
        if (verificadorInicio == True):
            if i>skip+1:
                with open("datosVerificados.txt","r") as g:
                    lVerif = g.readlines()
                    ultimoDato = lVerif[len(lVerif)-1]
                    anteUltimoDato = lVerif[len(lVerif)-2] 
            try:
                # verifica que cumpla con el formato xx.xx siendo x un entero
                if (int(l[:2]) and l[2:3]=="." and int(l[3:5])):
                    if (float(l) > 50.00):
                        if ("#" not in ultimoDato):
                            l = ultimoDato
                        else:
                            l=anteUltimoDato
                    with open("datosVerificados.txt","a") as f:
                        f.write("{0}".format(l))
                else:
                    int("error") # triggers except para evitar x.xxx
            except:
                with open("datosVerificados.txt","a") as f:                    
                    if ("#" in l and "#" not in ultimoDato):
                        f.write("{0}\n".format("#####"))
                    elif(i == skip): # Si el primer dato es erroneo guarda uno generico
                        f.write("{0}\n".format("20.00"))
                    elif (l[:1] == "." and l[4:5] != "." and "#" not in l and len(l) >= 5): # Si el primer digito es un . 
                        num = l[3:5] + "." + l[1:3] # .1234 ---> 34.12
                        f.write("{0}\n".format(num))
                    elif (l[4:5] == "." and l[:1] != "." and "#" not in l and len(l) >= 5): # 1234. ----> 34.12
                        num = l[2:4] + "." + l[:2]
                        f.write("{0}\n".format(num))
                    else:
                        if (1 < len(l) < 5 and pocosCaracteres == False):
                            pocosCaracteres = True
                            if ("#" not in ultimoDato):
                                # Si al dato le faltan caracteres, utiliza el anterior verificado
                                f.write("{0}".format(ultimoDato))
                            else: # Sino utiliza el anteultimo verificado
                                f.write("{0}".format(anteUltimoDato))
                                #print (anteUltimoDato)
                        elif (len(l) >= 5 or len(l) == 1):
                            if ("#" not in ultimoDato):
                                # Si al dato le faltan caracteres, utiliza el anterior verificado
                                f.write("{0}".format(ultimoDato))
                            else: # Sino utiliza el anteultimo verificado
                                f.write("{0}".format(anteUltimoDato))
                                #print (anteUltimoDato)
                        else:
                            pocosCaracteres = False
    '''datos = ""
    with open("datos.txt", "r") as f:
        lineas = f.readlines()
    for i in range(len(lineas)):
        l=lineas[i]
        ultimoDato="" # Dato anterior
        datos += l
        if (i%180 == 0):'''





def crearMatriz(): # Crea la matriz en el mismo orden de escaneo a partir de los datos sin errores
    print("Creando Matriz")
    matriz = np.full((36,179),20.00) #inicializo la matriz toda en temperatura ambiente
    volver = False
    columnas = 0
    filas = 0
    with open("datosVerificados.txt","r") as f:
        lineas = f.readlines()
        for l in lineas: 
            if ("#####" not in l[:-1]):              
                matriz[filas][columnas] = float(l[:-1])

                if (columnas == 178 and volver==False):
                    volver = True
                    columnas+=1

                if (columnas == 0 and volver==True):
                    volver = False
                    columnas-=1

                if (columnas < 178 and volver == False):
                    columnas=columnas+1
                else:
                    if (columnas > 0 and volver == True):
                        columnas = columnas - 1 #comienza a contar de zero de nuevo
            else:
                filas = filas + 1
    return matriz


def min_max(): # Calcula las temperatura minima y maxima de los datos verificados para mostrarlos en la pagina
    print ("Calculando minimo y maximo")
    minimo = 999
    maximo = -999
    with open("datosVerificados.txt", "r") as f:
        lineas = f.readlines()
    #print("minimo" + str(minimo))
    for l in lineas:
        try:
            l=float(l)
            if(l < minimo):
                minimo = l
                #print("minimo" + str(minimo))
                with open("temperaturas.txt", "w") as f:
                    f.write("{0}\n{1}\n".format(minimo,maximo))
            elif (l > maximo):
                maximo = l
                with open("temperaturas.txt", "w") as f:
                    f.write("{0}\n{1}\n".format(minimo,maximo))
        except:
            pass
    with open("temperaturas.txt", "a") as f: #guardo un fin en el archivo de temperaturas para habilitar el boton ver Resultado
        f.write("fin")

def enviar(comando):
    ser.write(comando)