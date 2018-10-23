from flask import Flask
from flask import render_template
from flask import request
import creador
import lector_datos as lector 

app = Flask(__name__)

@app.route('/')
def index():
    with open("temperaturas.txt", "w") as file: #Genera el archivo temperaturas.txt vacio
        file.write("")
    return render_template('index.html')

@app.route('/camara', methods = ['POST', 'GET'])
def camara():
    resultado=""
    with open("temperaturas.txt", "r") as file:
        lineas = file.readlines()
    if lineas:
        temperaturamin = lineas[0]
        temperaturamax = lineas[1]
        resultado = lineas[2]
    else:
        temperaturamin = 0
        temperaturamax = 0
    porcentaje = 100
    tiempo = 0

    # Si se termino de escanear y se genero el grafico entonces se habilita el boton de Ver Resultado
    if resultado == "fin":
        ver_resultado = ''' href=/resultado '''
    else:
        ver_resultado = ''' style= cursor:not-allowed disabled'''
    return render_template('camara.html',resultado=ver_resultado,tempmin = temperaturamin,
                            tempmax=temperaturamax,tiempo=tiempo,porcentaje=porcentaje)

@app.route('/ayuda')
def comoSeHace():
    return render_template('ayuda.html')

def cargarImagen():
    creador.crear_html_imagen()

@app.route('/lee_datos')
def lee_datos(): # Se utiliza para que ejecute la funcion leer datos    
    lector.iniciar()
    lector.leer()
    lector.verificar()
    lector.min_max()
    try:
        cargarImagen()
    except:
        pass
    return render_template('lee_datos.html')

@app.route('/quien-soy')
def quienSoy():
    return render_template('quien-soy.html')

@app.route('/resultado')
def resultado():
    return render_template('resultado.html')

if __name__ == "__main__":
    app.run (host = '127.0.0.1', port = 5000)
