#include <Adafruit_ADS1X15.h>
#include <Wire.h>
#include <math.h>
#include <ESP8266WiFi.h>

#include "funciones.h"

Adafruit_ADS1115 ads1X15; //Constructor del ads1115

#define PRINT_DEBUG_MESSAGES

//#define WiFi_CONNECTION_UPV
#define REST_SERVER_THINGSPEAK 

#ifdef WiFi_CONNECTION_UPV //Conexion UPV
  const char WiFiSSID[] = "GTI1";
  const char WiFiPSK[] = "1PV.arduino.Toledo";
#else //Conexion fuera de la UPV
  const char WiFiSSID[] = "mi wifi";
  const char WiFiPSK[] = "12345";
#endif
#if defined(WiFi_CONNECTION_UPV) //Conexion UPV
  const char Server_Host[] = "proxy.upv.es";
  const int Server_HttpPort = 8080;
#elif defined(REST_SERVER_THINGSPEAK) //Conexion fuera de la UPV
  const char Server_Host[] = "api.thingspeak.com";
  const int Server_HttpPort = 80;
#else
  const char Server_Host[] = "dweet.io";
  const int Server_HttpPort = 80;
#endif

WiFiClient client;
#ifdef REST_SERVER_THINGSPEAK 
  const char Rest_Host[] = "api.thingspeak.com";
  String MyWriteAPIKey="XYUYB9OUU5JBMEGB"; // Escribe la clave de tu canal ThingSpeak
#else 
  const char Rest_Host[] = "dweet.io";
  String MyWriteAPIKey="cdiocurso2021-2022g10"; // Escribe la clave de tu canal Dweet
#endif

#define NUM_FIELDS_TO_SEND 4 //Numero de medidas a enviar al servidor REST (Entre 1 y 8)
const int LED_PIN = 5;

void connectWiFi()
{
  byte ledStatus = LOW;

  #ifdef PRINT_DEBUG_MESSAGES
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
  #endif
  
  WiFi.begin(WiFiSSID, WiFiPSK);

  while (WiFi.status() != WL_CONNECTED)
  {
    // Blink the LED
    digitalWrite(LED_PIN, ledStatus); // Write LED high/low
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
    #ifdef PRINT_DEBUG_MESSAGES
       Serial.println(".");
    #endif
    delay(500);
  }
  #ifdef PRINT_DEBUG_MESSAGES
     Serial.println( "WiFi Connected" );
     Serial.println(WiFi.localIP()); // Print the IP address
  #endif
}
void HTTPPost(String fieldData[], int numFields){

// Esta funcion construye el string de datos a enviar a ThingSpeak mediante el metodo HTTP POST
// La funcion envia "numFields" datos, del array fieldData.
// Asegurate de ajustar numFields al número adecuado de datos que necesitas enviar y activa los campos en tu canal web
  
    if (client.connect( Server_Host , Server_HttpPort )){
       
        // Construimos el string de datos. Si tienes multiples campos asegurate de no pasarte de 1440 caracteres
   
        String PostData= "api_key=" + MyWriteAPIKey ;
        for ( int field = 1; field < (numFields + 1); field++ ){
            PostData += "&field" + String( field ) + "=" + fieldData[ field ];
        }     
        
        // POST data via HTTP
        #ifdef PRINT_DEBUG_MESSAGES
            Serial.println( "Connecting to ThingSpeak for update..." );
        #endif
        client.println( "POST http://" + String(Rest_Host) + "/update HTTP/1.1" );
        client.println( "Host: " + String(Rest_Host) );
        client.println( "Connection: close" );
        client.println( "Content-Type: application/x-www-form-urlencoded" );
        client.println( "Content-Length: " + String( PostData.length() ) );
        client.println();
        client.println( PostData );
        #ifdef PRINT_DEBUG_MESSAGES
            Serial.println( PostData );
            Serial.println();
            //Para ver la respuesta del servidor
            #ifdef PRINT_HTTP_RESPONSE
              delay(500);
              Serial.println();
              while(client.available()){String line = client.readStringUntil('\r');Serial.print(line); }
              Serial.println();
              Serial.println();
            #endif
        #endif
    }
}

void HTTPGet(String fieldData[], int numFields){
  
// Esta funcion construye el string de datos a enviar a ThingSpeak o Dweet mediante el metodo HTTP GET
// La funcion envia "numFields" datos, del array fieldData.
// Asegurate de ajustar "numFields" al número adecuado de datos que necesitas enviar y activa los campos en tu canal web
  
    if (client.connect( Server_Host , Server_HttpPort )){
           #ifdef REST_SERVER_THINGSPEAK 
              String PostData= "GET https://api.thingspeak.com/update?api_key=";
              PostData= PostData + MyWriteAPIKey ;
           #else 
              String PostData= "GET http://dweet.io/dweet/for/";
              PostData= PostData + MyWriteAPIKey +"?" ;
           #endif
           
           for ( int field = 1; field < (numFields + 1); field++ ){
              PostData += "&field" + String( field ) + "=" + fieldData[ field ];
           }
          
           
           #ifdef PRINT_DEBUG_MESSAGES
              Serial.println( "Connecting to Server for update..." );
           #endif
           client.print(PostData);         
           client.println(" HTTP/1.1");
           client.println("Host: " + String(Rest_Host)); 
           client.println("Connection: close");
           client.println();
           #ifdef PRINT_DEBUG_MESSAGES
              Serial.println( PostData );
              Serial.println();
              //Para ver la respuesta del servidor
              #ifdef PRINT_HTTP_RESPONSE
                delay(500);
                Serial.println();
                while(client.available()){String line = client.readStringUntil('\r');Serial.print(line); }
                Serial.println();
                Serial.println();
              #endif
           #endif  
    }
}


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
  #ifdef PRINT_DEBUG_MESSAGES
    Serial.begin(9600);
  #endif
  
  connectWiFi();
  digitalWrite(LED_PIN, HIGH);

  #ifdef PRINT_DEBUG_MESSAGES
      Serial.print("Server_Host: ");
      Serial.println(Server_Host);
      Serial.print("Port: ");
      Serial.println(String( Server_HttpPort ));
      Serial.print("Server_Rest: ");
      Serial.println(Rest_Host);
  #endif
  
  Serial.begin(9600); //Configura la velocidad de envio
  pinMode(power_pin, OUTPUT); //Ajusta el power_pin(pin 5) en modo salida
  ads1X15.begin(); //Inicia el ads1115
  ads1X15.setGain(GAIN_ONE); //Ajusta la ganancia del ads1115
  
 }

void loop(){
  if(cont1==0){
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

  int hola=20+random(0,10);
    String data[ NUM_FIELDS_TO_SEND + 1];  // Podemos enviar hasta 8 datos
    
    
    data[ 1 ] = String( 10 ); //Escribimos el dato 1. Recuerda actualizar numFields
    #ifdef PRINT_DEBUG_MESSAGES
        Serial.print( "Random1 = " );
        Serial.println( data[ 1 ] );
    #endif

    data[ 2 ] = String( 10 ); //Escribimos el dato 2. Recuerda actualizar numFields
    #ifdef PRINT_DEBUG_MESSAGES
        Serial.print( "Random2 = " );
        Serial.println( data[ 2 ] );
    #endif
    
     data[ 3 ] = String( 10 ); //Escribimos el dato 1. Recuerda actualizar numFields
    #ifdef PRINT_DEBUG_MESSAGES
        Serial.print( "Random1 = " );
        Serial.println( data[ 3 ] );
    #endif

    data[ 4 ] = String( hola); //Escribimos el dato 2. Recuerda actualizar numFields
    #ifdef PRINT_DEBUG_MESSAGES
        Serial.print( "Random2 = " );
        Serial.println( data[ 4 ] );
    #endif
    

    //Selecciona si quieres enviar con GET(ThingSpeak o Dweet) o con POST(ThingSpeak)
    //HTTPPost( data, NUM_FIELDS_TO_SEND );
    HTTPGet( data, NUM_FIELDS_TO_SEND );


    
  if(cont2==10){
    alerta=alertaSalinidad(mediaSalinidad); 
  }
  cont2=sleep(cont2,sleepTimeS);
}
