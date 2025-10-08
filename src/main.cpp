#include <Arduino.h>
#include <Ticker.h> //libreria para interrupciones temporizadas

Ticker miTicker;

// Pines de conexión 
#define DIR_PIN  18   // Dirección(DIR+)
#define STEP_PIN 19   // Paso(PUL+)
// Control de motor NEMA 23 con TB6600 y ESP32

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variables de control/////////////////////////////////////////////////
int velocidad = 100; // max (100) 1rpm
int acel = 100; //aceleracion no implementada(100) max
//variables fijas de motor y reduccion//////////////////////////////////
long pasos = 200;    // pasos a mover por vuelta
int reduccion = 50;
int ms = 2;   //diviciones del microstepping (1,2,4,8,16) !!AUMENTAR EL MICROSTEPPING DISMINUYE LA FUERZA DEL MOTOR A CAMBIO DE MAYOR PRECISION!!
//variables de posicionamiento///////////////////////////////////////////
int pos = 0; //posicion a 360° punto home
int p = 0; //posicion actual
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long hora = 12;
long minuto = 0;
long segundo = 0;


String mensaje; //variable para mensajes serial
int vel = 100/velocidad; //el 48 es un factor de corrección experimental (1rpm)
int contador = 0;  //contador para interrupciones ticker


void home(){
  //funcion para trackear home
}

void aumentar() {
  contador++;
}


/////////////////////////////////posteriormente incluir declinacion///////////////////////////////////////
void track(long h, long m, long s){
// Rutina de posicionamiento: ejecuta el movimiento solo si la coordenada de ascensión recta
// está entre 8h 0m 0s y 17h 59m 59s
  if ((h > 8 || (h == 8 && m >= 0 && s >= 0)) && (h < 17 || (h == 17 && m <= 59 && s <= 59))) {
    String AR[3]; // posición en Ascensión Recta
    AR[0] = h;
    AR[1] = m;
    AR[2] = s;

    long ir = (h * 15) + (m * 0.25) + (s * 0.004); // posición en grados
    long paso = (ir * pasos * reduccion * ms) / 360;

    if (paso > p)// si la nueva posición es mayor que la actual, avanzar, si es menor, retroceder
      digitalWrite(DIR_PIN, HIGH);
    else
      digitalWrite(DIR_PIN, LOW);

    for (long i = 0; i <= paso; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delay(vel);
      digitalWrite(STEP_PIN, LOW);
      delay(vel);
      if (i == paso) {
        p = paso;// actualizar posición actual
      }
    }
  }
  else {
    Serial.println("Fuera de rango de observacion");
  }
}

void follow(){
// rutina para seguimiento del astro
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(2, OUTPUT); //led integrado
  Serial.println("Listo. Comandos: avanzar, retroceder, velocidad<valor>, pasos<valor>, enable, disable");
  home();
  //track(hora);

  /////////////
  //miTicker.attach_ms(1000, aumentar);  // Llama “aumentar()” cada 1 segundo agregar al loop para la interrupcion 
  /////////////
}

void loop() {
  
  //rutina de seguimiento aun no programada
  //follow();

  //inicio de rutina de comunicacion serial
  if (Serial.available()) {
    mensaje = Serial.readStringUntil('\n');
    mensaje.trim();

    if (mensaje == "H") {
      // Lee la siguiente línea del puerto serie
      String horaStr = Serial.readStringUntil('\n');
      horaStr.trim();            // Elimina espacios o saltos de línea sobrantes
      hora = horaStr.toInt();       // Convierte el texto a número y lo guarda en hora
      // Limpia el buffer serial para preparar la siguiente lectura
      while (Serial.available() > 0) {
        Serial.read();           // Vacía cualquier carácter residual
      }
    }
    //repite para minuto y segundo
    if (mensaje == "M") {
      // le el valor del minuto
      String horaStr = Serial.readStringUntil('\n');
      horaStr.trim();
      minuto = horaStr.toInt();
      while (Serial.available() > 0) {
        Serial.read();
      }
    }
    if (mensaje == "S") {
      String horaStr = Serial.readStringUntil('\n');
      horaStr.trim();
      segundo = horaStr.toInt();
      while (Serial.available() > 0) {
        Serial.read();
      }
    }
    
    //ejeutamos el trackeo
    if (mensaje == "hornet") {
      Serial.println("Hora recibida: " + String(hora) + ":" + String(minuto) + ":" + String(segundo));
    }
  }
}

