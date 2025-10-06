#include <Arduino.h>
// Pines de conexión (ajusta según tu cableado)
#define DIR_PIN  18   // Dirección(DIR+)
#define STEP_PIN 19   // Paso(PUL+)
// Control de motor NEMA 23 con TB6600 y ESP32

// Variables de control
int velocidad = 50; // max (100) 1rpm
long pasos = 200;    // pasos a mover por comando
bool enable = true;
int ms = 2;   //400 pasos por vuelta 
int vel = 100/velocidad; //el 48 es un factor de corrección experimental (1rpm)
int pos = 0; //posicion a 360° punto home
long hora = 12;
int reduccion = 50;

void home(){
  //funcion para trackear home
}

void track(long tiempo){
  //traqueo
  long ir = tiempo*15; //posicion en grados
  long paso = (ir*pasos*reduccion*ms)/360;
  if (paso > 0)
  digitalWrite(DIR_PIN, HIGH);
  else
  digitalWrite(DIR_PIN, LOW);

  for (long i = 0; i <= paso; i++){
    digitalWrite(STEP_PIN, HIGH);
    delay(vel);
    digitalWrite(STEP_PIN, LOW);
    delay(vel);
  }

}

void follow(){

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  Serial.println("Listo. Comandos: avanzar, retroceder, velocidad<valor>, pasos<valor>, enable, disable");
  home();
  track(hora);
}

void loop() {
  
  //trackeo del astro
  follow();
  //seguimiento del astro

}

