#include <LiquidCrystal.h>
//-------------------Libreria para trabajar con multitarea-----------------------
#include "MultiTarea.h"
//-------------------------------------------------------------------------------
#include <Servo.h>
bool ledencendido = true;
int led = 9;
//--------------hay que declarar un objeto MultiTarea---------------------------
MultiTarea multi;
//-------------------------------------------------------------------------------
LiquidCrystal lcd(8, 3, 4, 2, 7, 6);
Servo myservo;    // create servo object to control a servo
int potpin = A0;  // analog pin used to connect the potentiometer
int val;          // variable to read the value from the analog pin

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(led, OUTPUT);
  myservo.attach(10);  // attaches the servo on pin 9 to the servo object-----
                       //--------------------Se agregan las funciones que van a trabajar en paralelo---
                       //------y se define cada cuanto tiempo se van a llamar (en milisegundos)--------
  multi.AgregaTarea(parpadea, 100); //parpadea un led cada 100 milisegundos
  multi.AgregaTarea(mensaje, 3000); //manda un mensaje por el puerto serial cada 3 segundos
  multi.AgregaTarea(cronometro, 1000); //muestra un cronometro por el LCD cada segundo
  multi.AgregaTarea(ServoMotor, 15); //controla el servomotor cada 15 milisegundos
  //-------------------------------------------------------------------------
  Serial.println("hola");
}

// the loop function runs over and over again forever
void loop() {
  //---------------------Se inicia la multitarea-----------------------------
  multi.Ejecuta();
  //--------------------------------------------------------------------------
}
//---------------------funcion que manda un mensaje por el puerto serial-----
void mensaje() {
  int static llamada = 0;
  Serial.println("Hola desde el segundo hilo: " + String(llamada));
  llamada++;
}
// -----------------funcion que hace parpadear el led-----------------------
void parpadea() {
  if (ledencendido == true) {
    digitalWrite(led, LOW);  // turn the LED off by making the voltage LOW
    ledencendido = false;
  } else {
    digitalWrite(led, HIGH);  // turn the LED on (HIGH is the voltage level)
    ledencendido = true;
  }
}
//-----------Cronometro que se muestra por el LCD--------------------------
void cronometro() {
  int static segundos = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Segundos:" + String(segundos));
  segundos++;
}
//------------funcion que controla el servomotor-----------------------------
void ServoMotor() {
  val = analogRead(potpin);
  val = map(val, 0, 1023, 0, 180);
  myservo.write(val);
  //----------Es importante no usar la funcion delay------------------
  //  delay(15);
}