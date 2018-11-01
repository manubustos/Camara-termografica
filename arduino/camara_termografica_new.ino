#include <Servo.h>
#include <Adafruit_MLX90614.h>
#include <FiniteStateMachine.h>
//variables de uso
Servo servoEjeX;
Servo servoEjeY;
Adafruit_MLX90614 sensor = Adafruit_MLX90614();
int desplazamiento;
int contEscaneo;
boolean flagPositivo; //variable que manejaria la direccion a la que va el servo X
boolean flagTerminoDesplazamientoX;
State Init (iniciar ());
State Start(comenzar());
State Pause(pausa());
State Stop(reiniciar());
//abajo iria la variable que toma el valor de la entrada a la maquina desde la pagina
//3 posibles valores:
//*la persona apreto comenzar en la pagina
//*la persona apreto pausa
//*la persona apreto detener
//como idea de funcionalidad, las opciones pausar y detener no deberian estar habilitadas hasta que
//se haya presionado comenzar
enum states{
    inicio,
    comenzar,
    pausar,
    detener
};
states estadoActual;

void setup() {
    FSM stateMachine = FSM(Init);
    estadoActual=inicio;
}
void loop{
    //2do problema encontrado: necesitamos 4 estados? el detener vuelve al estado inicial
    //3er problema encontrado: como obtener el estadoActual dada la presion de un boton en la pagina web
    //solucion 3er problema: onclick event:  <button onclick="myFunction()">Click me</button> 
    estadoActual=actualizarEstado();
    switch (estadoActual){
        case "comenzar": stateMachine.transitionTo(Start);break;
        case "pausa": stateMachine.transitionTo(Pause);break;
        case "detener": stateMachine.transitionTo(Stop);break; //???
    }
    stateMachine.update();
}

void iniciar(){
    // Inicia los servos para que comiencen a funcionar con los respectivos pines
    servoEjeX.attach(10);
    servoEjeY.attach(3);
    // Inicializa servo ejeX en 0°
    servoEjeX.write(0);
    // Inicializa servo ejeY en 180° para ir escaneando de abajo para arriba
    servoEjeY.write(180);
    
     // configuracion led
    pinMode(6,OUTPUT);
    pinMode(5,OUTPUT);
    digitalWrite(ledAvisoD,LOW); // el led comienza apagado
    digitalWrite(ledAvisoC,LOW); // el led comienza apagado
    avisoEncendido();

    desplazamiento = 180; //variable global que ira descontando la posicion del servo del ejeY
    flagPositivo=true;//variable que me indica que el movimiento sera hacia la derecha
    flagTerminoDesplazamientoX=false; //variable que me permite saber cuando termino un desplazamiento en el eje X.
    contEscaneo=0;//contador para la cantidad de veces que lo tengo que hacer
}
void avisoEncendido(){
    digitalWrite(ledAvisoC,HIGH);
    delay(2500);
    digitalWrite(ledAvisoC,LOW);
    delay(300);
}


void comenzar(){
    escanearX();
    if (flagTerminoDesplazamientoX){
        desplazarY();
        Serial.print("#####");
    }
}
//--------------------------------------------Funciones de escaneo----------------------------------------------------------------------------
void escaneoPositivo(){
    //funcion que escanea en la direccion positiva. Cuando termina el escaneo habilita el escaneo negativo
    //e indica por medio de un flag que termino el desplazamiento en X, para que pueda ocurrir el desplazamiento en Y
    Serial.print(sensor.readObjectTempC()); // Leo temperatura °C y la envio al puerto serie
    contEscaneo++;
    if (contEscaneo!=179){
        servoEjeX.write(contEscaneo); // Desplaza al ángulo correspondiente
    }else{
        flagPositivo=false;
        flagTerminoDesplazamientoX=true;
    }
    delay(40); //ni puta idea de por que este delay
    }
}
void escaneoNegativo(){
    //funcion que escanea en la direccion negativa. Cuando termina el escaneo habilita el escaneo positivo
    //e indica por medio de un flag que termino el desplazamiento en X, para que pueda ocurrir el desplazamiento en Y
    Serial.print(sensor.readObjectTempC()); // Leo temperatura °C y la envio al puerto serie
    contEscaneo--;
    if (contEscaneo!=0){
        servoEjeX.write(contEscaneo); // Desplaza al ángulo correspondiente
    }else{
        flagPositivo=true;
        flagTerminoDesplazamientoX=true;
    }
    delay(40); //ni puta idea de por que este delay
    }
}
void escanearX(){
    //1er problema encontrado, como hacer para detener al Estado debido a que ejecuta todo un for de 0 a 179
    //solucion planteada: eliminar los fors y whiles por ifs
    //funcion que controla el desplazamiento y el escaneo de la temperatura en el eje X
    if (flagPositivo){
        escaneoPositivo();
    }else{
        escaneoNegativo();
    }  
  }
}
void desplazarY(){
    //funcion que controla el desplazamiento del servoY
    //cuando termino de desplazarse en el eje Y vuelve a poner el flagTerminoDesplazamientoX en falso
    desplazamiento = desplazamiento-5;
    servoEjeY.write(desplazamiento);
    flagTerminoDesplazamientoX=false;
}
void reiniciar(){
    // Inicia los servos para que comiencen a funcionar con los respectivos pines
    servoEjeX.attach(10);
    servoEjeY.attach(3);
    // Inicializa servo ejeX en 0°
    servoEjeX.write(0);
    // Inicializa servo ejeY en 180° para ir escaneando de abajo para arriba
    servoEjeY.write(180);
    
    FSM stateMachine = FSM(Init);
    estadoActual=inicio;
}