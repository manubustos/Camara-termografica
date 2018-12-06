from flask import Flask, render_template, request, jsonify
import creador
import lector
import serial

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/camara')
def camara():
    return render_template('camara.html')

@app.route('/escaner')
def escaner():
    return render_template('escaner.html')

@app.route('/ayuda')
def comoSeHace():
    return render_template('ayuda.html')

@app.route('/enviarArduino', methods=['POST'])
def enviarArduino():
    comando = request.form['comando']
    if comando == '1':
        lector.enviar('1')
        lector.leer()

    if comando == '2':
        lector.enviar('2')

    if comando == '3':
        lector.enviar('3')

    return '', 204

@app.route('/getPorcentaje', methods = ['GET'])
def getPorcentaje():
    porc = lector.getPorcentaje()
    return jsonify({"porcentaje": porc})

@app.route('/quien-soy')
def quienSoy():
    return render_template('quien-soy.html')

if __name__ == "__main__":
    app.run (host = '127.0.0.1', port = 5000)
