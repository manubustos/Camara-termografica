/* CAMARA TERMOGRAFICA SIN CAMARA
 * Proyecto para la materia Taller de Proyecto II.
 * Sistema con conexion con un servidor web donde se mostrará imagen escaneada con las distintas temperaturas.
 * 
 * VERSION 1.0
 * 
 * Cambios: 
 * se implemento el uso de dos botones fisicos (comenzar y detener)
 * el arduino no leerá datos que vengan del puerto serie
 * 
 * sistema de deteccion de errores con leds
*/
// Incluye las librerías para poder controlar los servos y el sensor.
#include <Servo.h>
#include <Adafruit_MLX90614.h>

// Variables para controlar los servos
Servo servoEjeX;
Servo servoEjeY;

Adafruit_MLX90614 sensor = Adafruit_MLX90614();

int desplazamiento;

// Variables para controlar boton de comenzar
const int botonPinC = 2;
int estadoActualBotonComenzar = HIGH;
int estadoAnteriorBotonComenzar = HIGH;
boolean comenzo;

// Variables para controlar boton de Detener
const int botonPinD = 4;
int estadoActualBotonDetener = HIGH;
int estadoAnteriorBotonDetener = HIGH;
boolean detener;

const int ledAvisoC = 6; // pin del led que indica errores / encendido del aparato / finalizado de escaneo
const int ledAvisoD = 5; // pin del led que indica errores / encendido del aparato / finalizado de escaneo

void setup() {

  Serial.begin(9600);
  sensor.begin(); 

  pinMode(botonPinD,INPUT_PULLUP); // boton de comenzar
  pinMode(botonPinC,INPUT_PULLUP); // boton de detener
 
  // Inicia los servos para que comiencen a funcionar con los respectivos pines
  servoEjeX.attach(10);
  servoEjeY.attach(3);
  // Inicializa servo ejeX en 0°
  servoEjeX.write(0);
  // Inicializa servo ejeY en 180° para ir escaneando de abajo para arriba
  servoEjeY.write(180);
  desplazamiento = 180; //variable global que ira descontando la posicion del servo del ejeY
  
  comenzo = false; // para evitar que cuando apreten muchas veces el boton de comenzar no suceda nada
  detener = false; // para saber si se presiono el boton de detener

  // configuracion led
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(ledAvisoD,LOW); // el led comienza apagado
  digitalWrite(ledAvisoC,LOW); // el led comienza apagado
  avisoEncendido();  
}

void loop() {
  if(esperarComienzo() == true){ // significa que toco el boton de comenzar una vez
    escanear();    
  }
}

boolean esperarComienzo(){
  estadoActualBotonComenzar = digitalRead(botonPinC); // leo el estado actual del boton comenzar 
  if(estadoActualBotonComenzar != estadoAnteriorBotonComenzar || comenzo == true){ // significa que toco el boton de comenzar una vez
    detener = false; //reseteo la variable por si se ecanea sin apagar el dispositivo
    comenzo = true; // Por si presionan mas de una vez el boton de comenzar, no lo tendra en cuenta
    digitalWrite(ledAvisoC,HIGH); // prendo el led para avisar que comenzo a escanear
    return true;    
  }else return false;
}

// ------------------------------- FUNCIONES PARA DETENER ESCANEO ----------------------------------------------

boolean detectarBotonDetener(){
  estadoActualBotonDetener = digitalRead(botonPinD); // leo el estado actual del boton de detener 
  if(estadoActualBotonDetener != estadoAnteriorBotonDetener){
    detener = true;
    comenzo = false;
    estadoActualBotonComenzar = estadoAnteriorBotonComenzar; // vuelven a ser iguales de nuevo para que pueda comenzar otra vez
    estadoActualBotonDetener = estadoAnteriorBotonDetener;
    digitalWrite(ledAvisoC,LOW);
    for(int i=0; i<3; i++){ // titilara y se apagar para indicar que terminó
      digitalWrite(ledAvisoD,HIGH);
      delay(300);
      digitalWrite(ledAvisoD,LOW);
      delay(300);      
    }
    volverACasa();    
    // enviar señal de finalizacion al servidor
    for(int i=0; i<50;i++){
      Serial.print("@@");
    }            
    return true;
  }else return false;
}

// ------------------------- FUNCIONES DE ESCANEO ------------------------------------

void escanearPositivo(){
  // Sentido positivo
  for (int i = 0; i <= 179; i=i+1){ //hace que vuelva al principio
    if (detectarBotonDetener()){
      break; // si detecta que se presiono el boton de detener termina de escanear
    }
    Serial.print(sensor.readObjectTempC()); // Leo temperatura °C y la envio al puerto serie
    if(i!=179){
      servoEjeX.write(i); // Desplaza al ángulo correspondiente
    }
    //delay(500); // Pausa de 25ms
    delay(40);
    //delay(45);
  }
}
void escanearNegativo(){
  // Para el sentido negativo
  for (int i = 179; i >= 0; i=i-1){ 
    if (detectarBotonDetener()){
      break; // si detecta que se presiono el boton de detener termina de escanear
    }
    Serial.print(sensor.readObjectTempC()); // Leo temperatura °C y la envio al puerto serie
    if (i != 0){
      servoEjeX.write(i); // Desplaza al ángulo correspondiente
    }    
    //delay(500); // Pausa de 25ms
    delay(45);
    //delay(45);
  }
}
void escanear(){ // Futura mejora, implementar un thread para verificar el boton de detener.
  // Dos bucles: uno para mover en sentido positivo y otro en sentido negativo
  escanearPositivo();
  if (detener == false){ // verifico que no se haya presionado el boton de detener       // Averiguar si cuando apreto dos veces el boton cambia o no. sino pregunto con la funcion
    // Desplazo el eje Y
    desplazamiento = desplazamiento-5;
    servoEjeY.write(desplazamiento);
    Serial.print("#####");
    escanearNegativo();
    Serial.print("#####");
      if (detener == false){
        // Desplazo el eje Y
        desplazamiento = desplazamiento-5;
        servoEjeY.write(desplazamiento);
        if (desplazamiento == 0){ // Finalizo el escaneo
          estadoActualBotonComenzar = estadoAnteriorBotonComenzar; // vuelven a ser iguales de nuevo para que pueda comenzar otra vez
          estadoActualBotonDetener = estadoAnteriorBotonDetener;
          volverACasa();
          detener = true;
          comenzo = false;
          for(int i=0; i<50;i++){
            Serial.print("@@");
          }          
        }  
      } 
    }
  }

//------------------------- FUNCION VOLVER AL COMIENZO ------------------------------------

void volverACasa(){
  // Inicializa servo ejeX en 0°
  servoEjeX.write(0);
  // Inicializa servo ejeY en 180° para ir escaneando de abajo para arriba
  servoEjeY.write(180);
  desplazamiento = 180; //variable global que ira descontando la posicion del servo del ejeY
}


// --------------------------- FUNCIONES LED --------------------------------------------
void avisoEncendido(){
  digitalWrite(ledAvisoC,HIGH);
  delay(2500);
  digitalWrite(ledAvisoC,LOW);
  delay(300);
}
