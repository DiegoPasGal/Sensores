#include <Adafruit_ADS1X15.h>
#include <Wire.h>
#include <math.h>

#include "funciones.h"

Adafruit_ADS1115 ads1X15; //Constructor del ads1115

//SENSOR DE HUMEDAD
int16_t adc1;
int16_t humedad; //Valor de la humedad
int AirValue; //Valor del sensor de humedad en contacto con el aire
int WaterValue; //Valor del sensor de humedad en contacto con el agua

//SENSOR DE SALINIDAD
int power_pin=5; //Utilizamos el pin 5 para alimentar la sonda de salinidad
int adc0;
int salinidad; //Valor de la salinidad
int SalValue=0; //Valor del sensor de salinidad en contacto con la sal
int NoSalValue=0; //Valor del sensor de salinidad cuando no esta en contacto con la sal

//SENSOR DE TEMPERATURA
int16_t adc2;
float temperatura; //Valor de la temperatura
float b = 1.0527; //Constante uilizada para calcular la temperaura 
float m = 0.0227; //Constante uilizada para calcular la temperaura 

//SENSOR DE ILUMINACION
int16_t adc3;
int LightValue; //Valor del sensor de iluminacion en contacto con luz
int iluminacion; //Valor de la iluminacion

//contadores
int cont1=0; //Controla si se ha ejecutado el primer if en el loop (que solo se tiene que ejecutar una vez)
int cont2=1; //MUY IMPORTANTE NO TOCAR. Controla cuantas veces se han leido los datos, media() y el sleep()
int inicioC=0;

//time sleep
const int sleepTimeS = 30; //Segundos que se ejecuta el deepSleep

//alerta salinidad en el agua
bool alerta; //true=demasiada sal, false=nivel de sal normal

//medias
int mediaHumedad;
int mediaSalinidad;
float mediaTemperatura;
int mediaIluminacion;

void setup(){
  Serial.begin(9600); //Configura la velocidad de envio
  pinMode(power_pin, OUTPUT); //Ajusta el power_pin(pin 5) en modo salida
  ads1X15.begin(); //Inicia el ads1115
  ads1X15.setGain(GAIN_ONE); //Ajusta la ganancia del ads1115
 }

void loop(){
  inicioC = inicio(inicioC);
  if(inicioC==1){
    delay(5000); //Esperamos 5s
    //CALIBRAR HUMEDAD
    adc1 = ads1X15.readADC_SingleEnded(1);//adc1= al valor que lea el ads1115 en el pin A1 SENSOR DE HUMEDAD
    calibrarHumedad1(adc1,&AirValue,&WaterValue);
    adc1 = ads1X15.readADC_SingleEnded(1);//adc1= al valor que lea el ads1115 en el pin A1 SENSOR DE HUMEDAD
    calibrarHumedad2(adc1,&AirValue,&WaterValue);
    //CALIBRAR SALINIDAD
    digitalWrite(power_pin,HIGH);
    delay(100);//espera 100 ms
    adc0 = ads1X15.readADC_SingleEnded(0);//adc0= al valor que lea el ads1115 en el pin A0 SENSOR DE SALINIDAD
    digitalWrite(power_pin,LOW);
    calibrarSalinidad1(adc0,&SalValue,&NoSalValue);
    digitalWrite(power_pin,HIGH);
    delay(100);//espera 100 ms
    adc0 = ads1X15.readADC_SingleEnded(0);//adc0= al valor que lea el ads1115 en el pin A0 SENSOR DE SALINIDAD
    digitalWrite(power_pin,LOW);
    calibrarSalinidad2(adc0,&SalValue,&NoSalValue);
    //CALIBRAR ILUMINACION
    adc3=ads1X15.readADC_SingleEnded(3);
    calibrarIluminacion(adc3,&LightValue);
    cont1=1;
    inicioC=0;
  }
  //CALIBRACIONES AUTOMATICAS
  adc1 = ads1X15.readADC_SingleEnded(1);//adc1= al valor que lea el ads1115 en el pin A1 SENSOR DE HUMEDAD
  calibrarHumedadAutomatica(adc1,&AirValue,&WaterValue);
  adc1 = ads1X15.readADC_SingleEnded(1);//adc1= al valor que lea el ads1115 en el pin A1 SENSOR DE HUMEDAD
  digitalWrite(power_pin,HIGH);
  delay(100);//espera 100 ms
  adc0 = ads1X15.readADC_SingleEnded(0);//adc0= al valor que lea el ads1115 en el pin A0 SENSOR DE SALINIDAD
  digitalWrite(power_pin,LOW);
  calibrarSalinidadAutomatica(adc0,&SalValue,&NoSalValue);
  adc3=ads1X15.readADC_SingleEnded(3);
  calibrarIluminacionAutomatica(adc3,&LightValue);
  //Empieza la lectura de datos
  delay(2000); //Esperamos 2s
  Serial.print("  ");
  Serial.print(cont2);
  Serial.println("º medida");
  Serial.println("  ____________________________________________________");
  Serial.println("");
  //LEE LA HUMEDAD
  humedad=leerHumedad(AirValue,WaterValue,adc1);
  //LEE LA SALININAD
  salinidad=leerSalinidad(SalValue,NoSalValue,adc0);
  //LEE LA ILUMINACION
  iluminacion=leerIluminacion(adc3,LightValue);
  //LEE LA TEMPERATURA
  temperatura=leerTemperatura(adc2,b,m);
  Serial.println("");
  //MEDIAS Y DEEP SLEEP
  media(cont2,humedad,&mediaHumedad,salinidad,&mediaSalinidad,temperatura,&mediaTemperatura,iluminacion,&mediaIluminacion);
  if(cont2==10){
    alerta=alertaSalinidad(mediaSalinidad); 
  }
  cont2=sleep(cont2,sleepTimeS);
}
