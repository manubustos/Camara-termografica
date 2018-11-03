#include <Servo.h>
#include "Adafruit_MLX90614.h"

Servo servoEjeX;
Servo servoEjeY;
Adafruit_MLX90614 sensor = Adafruit_MLX90614();

int desplazamiento;
int contEscaneo;

boolean flagPositivo;
boolean flagTerminoDesplazamientoX;

enum State
{
    Init,
    Wait,
    Scan
};

enum Input
{
    Unknown,
    Start,
    Pause,
    Continue,
    Stop
};

State currentState;
Input currentInput;

const int ledAvisoC = 6;
const int ledAvisoD = 5;

void setup()
{
    currentState = State::Init;
}

void loop()
{

    switch (currentState)
    {
        case State::Init:
            start();
            break;
        case State::Wait:
            delay(100);
            break;
        case State::Scan:
            escanear();
        default:
            break;
    }

    if(readInput())
        MEF_Update();
}

void start(){

    Serial.begin(9600);

    // Inicia los servos para que comiencen a funcionar con los respectivos pines
    servoEjeX.attach(10);
    servoEjeY.attach(3);

    // Inicializa servo ejeX en 0° y el ejeY en 180°
    servoEjeX.write(0);
    servoEjeY.write(180);
    
     // Inicia los leds como salida y para que comiencen apagados
    pinMode(6,OUTPUT);
    pinMode(5,OUTPUT);
    digitalWrite(ledAvisoD,LOW);
    digitalWrite(ledAvisoC,LOW);
    avisoEncendido();

    desplazamiento = 180; //variable global que ira descontando la posicion del servo del ejeY
    flagPositivo = true;//variable que me indica que el movimiento sera hacia la derecha
    flagTerminoDesplazamientoX = false; //variable que me permite saber cuando termino un desplazamiento en el eje X.
    contEscaneo = 0;//contador para la cantidad de veces que lo tengo que hacer

    currentState = State::Wait;
}

void avisoEncendido()
{
    // Enciendo el led
    digitalWrite(ledAvisoC,HIGH);
    delay(2500);

    // Apago el led
    digitalWrite(ledAvisoC,LOW);
    delay(300);
}

void escanear()
{
    escanearX();

    if (flagTerminoDesplazamientoX)
    {
        desplazarY();
        Serial.print("#####");
    }
}

//--------------Funciones de escaneo--------------//

void escanearX()
{
    if (flagPositivo)
        escaneoPositivo();
    else
        escaneoNegativo();
}

/* Funcion que escanea en la direccion positiva.
 * Cuando termina el escaneo habilita el escaneo negativo e indica por medio de un flag
 * que termino el desplazamiento en X.*/
void escaneoPositivo()
{
    Serial.print(sensor.readObjectTempC()); // Leo temperatura °C y la envio al puerto serie

    contEscaneo++;
    if (contEscaneo!=179)
        servoEjeX.write(contEscaneo); // Desplaza al ángulo correspondiente
    else
    {
        flagPositivo=false;
        flagTerminoDesplazamientoX=true;
    }

    delay(40);
}

/* Funcion que escanea en la direccion negativa.
 * Cuando termina el escaneo habilita el escaneo positivo e indica por medio de un flag
 * que termino el desplazamiento en X.*/
void escaneoNegativo()
{
    Serial.print(sensor.readObjectTempC()); // Leo temperatura °C y la envio al puerto serie

    contEscaneo--;
    if (contEscaneo!=0)
        servoEjeX.write(contEscaneo); // Desplaza al ángulo correspondiente
    else
    {
        flagPositivo=true;
        flagTerminoDesplazamientoX=true;
    }

    delay(40);
}

/* Funcion que controla el desplazamiento del servoY.
 * Cuando termino de desplazarse en el eje Y vuelve a poner el flag
 * TerminoDesplazamientoX en falso*/
void desplazarY()
{
    desplazamiento = desplazamiento-5;
    servoEjeY.write(desplazamiento);
    flagTerminoDesplazamientoX=false;
}

//--------------Función de lectura del puerto serie--------------//
boolean readInput()
{
    currentInput = Input::Unknown;

    if (Serial.available())
    {
        char incomingChar = Serial.read();

        switch (incomingChar)
        {
            case '1': currentInput = Input::Start; break;
            case '2': currentInput = Input::Pause; break;
            case '3': currentInput = Input::Stop; break;
            default: break;
        }
        return true;
    }
    return false;
}

//--------------Función para cambiar el estado y realizar llamar a las funciones--------------//
void MEF_Update()
{
    switch (currentState)
    {
    case State::Init: currentState = State::Wait; break;
    case State::Wait:
        if(currentInput == Input::Start)
            currentState = State::Scan;
        else if(currentInput == Input::Stop)
            currentState = State::Init;
        break;
    case State::Scan:
        if(currentInput == Input::Pause)
            currentState = State::Wait;
        else if(currentInput == Input::Stop)
            currentState = State::Init;
        break;
    default:
        break;
    }
}
