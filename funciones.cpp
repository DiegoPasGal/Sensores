#include <Arduino.h>
#include <math.h>
#include "funciones.h"

//---------------------------------------------------------------------------------------------------------------------------------------
// inicio()
//---------------------------------------------------------------------------------------------------------------------------------------

int inicio(int inicioC){
  char data=0;
  int pausa=0;
  if(pausa==0){
    delay(1000); //Esperamos 1s
  }
  while(pausa!=1){
    delay(500);
    data = 0;
    data = Serial.read();
    delay(500);
    Serial.print("  .");
      if (data == '1'){
        pausa = 1;
        inicioC =1;
      }
      if (data == '2'){
        pausa = 1;
        inicioC = 2;
      }
   }
   Serial.println("");
   Serial.println("");
   return inicioC;
}

//---------------------------------------------------------------------------------------------------------------------------------------
// calibrarHumedad1
//---------------------------------------------------------------------------------------------------------------------------------------

void calibrarHumedad1(int16_t adc1, int * AirValue, int * WaterValue){
  int pausa=0;
  
  Serial.println("  Empezamos calibrando el sensor de humedad.");
  delay(1000); //Esperamos 1s
  Serial.println("  Asegurese de tener el sensor de humedad en contacto con el aire.");
  Serial.println("");
  pausar(pausa);
  pausa=0;
  *AirValue=adc1;
  Serial.print("  AirValue: ");
  Serial.println(*AirValue);
  Serial.println("");
}

//---------------------------------------------------------------------------------------------------------------------------------------
// calibrarHumedad2
//---------------------------------------------------------------------------------------------------------------------------------------

void calibrarHumedad2(int16_t adc1, int * AirValue, int * WaterValue){
  int pausa=0;

  delay(1000); //Esperamos 1s
  Serial.println("  A continuación vamos a calibrar el sensor de humedad en contacto con el agua.");
  delay(1000); //Esperamos 1s
  Serial.println("  Sumerja el sensor en un recipiente con agua hasta la marca indicada en el sensor.");
  Serial.println("");
  pausar(pausa);
  pausa=0;
  *WaterValue=adc1;
  Serial.print("  WaterValue: ");
  Serial.println(*WaterValue);
  Serial.println("");
}

//---------------------------------------------------------------------------------------------------------------------------------------
// calibrarHumedadAutomatica
//---------------------------------------------------------------------------------------------------------------------------------------

void calibrarHumedadAutomatica(int16_t adc1, int * AirValue, int * WaterValue){
  if(adc1>*AirValue){
    *AirValue=adc1;
  }
  if(adc1<*WaterValue){
    *WaterValue=adc1;
  }
}

//---------------------------------------------------------------------------------------------------------------------------------------
// calibrarSalinidad1
//---------------------------------------------------------------------------------------------------------------------------------------

void calibrarSalinidad1(int adc0,int * SalValue,int * NoSalValue){
  int pausa=0;

  delay(1000); //Esperamos 1s
  Serial.println("  Ahora vamos a calibrar el sensor de salinidad caundo no esta en contacto con sal.");
  delay(1000); //Esperamos 1s
  Serial.println("  Introduzca los dos cables en un recipiente con agua destilada o con la menor cantidad de sal posible");
  Serial.println("");
  pausar(pausa);
  pausa=0;
  *NoSalValue=adc0;
  Serial.print("  NoSalValue: ");
  Serial.println(*NoSalValue);
  Serial.println("");
}

//---------------------------------------------------------------------------------------------------------------------------------------
// calibrarSalinidad2
//---------------------------------------------------------------------------------------------------------------------------------------

void calibrarSalinidad2(int adc0,int * SalValue,int * NoSalValue){
  int pausa=0;

  delay(1000); //Esperamos 1s
  Serial.println("  A continuación vamos a calibrar el sensor de salinidad en contacto con sal.");
  delay(1000); //Esperamos 1s
  Serial.println("  Introduzca los dos cables en un recipiente con agua con una cantidad de sal considerable.");
  Serial.println("");
  pausar(pausa);
  pausa=0;
  *SalValue=adc0;
  Serial.print("  SalValue: ");
  Serial.println(*SalValue);
  Serial.println("");
}

//---------------------------------------------------------------------------------------------------------------------------------------
// calibrarSalinidad2
//---------------------------------------------------------------------------------------------------------------------------------------

void calibrarSalinidadAutomatica(int adc0,int * SalValue,int * NoSalValue){
 if(adc0<*NoSalValue){
    *NoSalValue=adc0;
   }
    if(adc0>*SalValue){
    *SalValue=adc0;
   }
}

//---------------------------------------------------------------------------------------------------------------------------------------
// calibrarIluminacion
//---------------------------------------------------------------------------------------------------------------------------------------

void calibrarIluminacion(int16_t adc3,int * LightValue){
  int pausa=0;

  delay(1000); //Esperamos 1s
  Serial.println("  Vamos a calibrar el sensor de iluminación.");
  delay(1000); //Esperamos 1s
  Serial.println("  Acerque una linterna al fotodiodo.");
  Serial.println("");
  pausar(pausa);
  pausa=0;
  *LightValue=adc3;
  Serial.print("  LightValue: ");
  Serial.println(*LightValue);
  Serial.println("");
}

//---------------------------------------------------------------------------------------------------------------------------------------
// calibrarIluminacionAutomatica
//---------------------------------------------------------------------------------------------------------------------------------------

void calibrarIluminacionAutomatica(int16_t adc3,int * LightValue){
 if(adc3>*LightValue){
    *LightValue=adc3;
  }
}

//---------------------------------------------------------------------------------------------------------------------------------------
// leerHumedad()
//---------------------------------------------------------------------------------------------------------------------------------------

int leerHumedad(int AirValue,int WaterValue,int16_t adc1){
  int humedad = 100*AirValue/(AirValue-WaterValue)-adc1*100/(AirValue-WaterValue);

  if(humedad<0){
    humedad=0;
  }
  if(humedad>100){
    humedad=100;
  }
  Serial.print("  HUMEDAD : ");
  Serial.print(humedad);
  Serial.print("%");
  if(humedad>=0 && humedad<10){
    Serial.println("    Seco");
  }
  else if(humedad>=10 && humedad<=80){
    Serial.println("    Mojado");
  }
  else if(humedad>80 && humedad<=100){
    Serial.println("    Muy mojado");
  }
  return humedad;
}

//---------------------------------------------------------------------------------------------------------------------------------------
// leerSalinidad()
//---------------------------------------------------------------------------------------------------------------------------------------

int leerSalinidad(int SalValue,int NoSalValue,int16_t adc0){
  int salinidad = 100*NoSalValue/(NoSalValue-SalValue)-adc0*100/(NoSalValue-SalValue);
  
  if(salinidad<0){
    salinidad=0;
  }
  if(salinidad>100){
    salinidad=100;
  }
  Serial.print("  SALINIDAD : ");
  Serial.print(salinidad);
  Serial.print("%");
  if(salinidad>=0 && salinidad<10){
   Serial.println("    Sin sal");
  }
  else if(salinidad>=10 && salinidad<=80){
   Serial.println("    Salado");
  }
  else if(salinidad>80 && salinidad<=100){
   Serial.println("    Muy salado");
  }
  return salinidad;
}

//---------------------------------------------------------------------------------------------------------------------------------------
// leerTemperatura()
//---------------------------------------------------------------------------------------------------------------------------------------

int leerTemperatura(int16_t adc2, float b, float m){
  float temperatura = (adc2*4.096)/(pow(2,15)-1);
  float temperaturaCelsius = -0.7+((temperatura-b)/m);
  float temperaturaKelvin = temperaturaCelsius+273.15;
  float temperaturaFahrenheit = (temperaturaCelsius*1.8)+32;
  Serial.print("  TEMPERATURA :");
  Serial.print(" ");
  Serial.print(temperaturaCelsius);
  Serial.println(" ºC (Grados Celsius)");
  Serial.print("                ");
  Serial.print(temperaturaFahrenheit);
  Serial.println(" ºF (Grados Fahrenheit)");
  Serial.print("                ");
  Serial.print(temperaturaKelvin);
  Serial.println(" K (Grados Kelvin)");
  return temperaturaCelsius;
}

//---------------------------------------------------------------------------------------------------------------------------------------
// leerIluminacion()
//---------------------------------------------------------------------------------------------------------------------------------------

int leerIluminacion(int16_t adc3,int LightValue){
  int  iluminacion=(adc3*100)/LightValue;

  if(iluminacion<0){
    iluminacion=0;
  }
  if(iluminacion>100){
    iluminacion=100;
  }
  Serial.print("  ILUMINACIÓN : ");
  Serial.print(iluminacion);
  Serial.print("%");
  if(iluminacion>=0 && iluminacion<=10){
    Serial.println("    Oscuro o poco iluminado");
  }
  if(iluminacion>10 && iluminacion<75){
    Serial.println("    Iluminado");
  }
  if(iluminacion>=75 && iluminacion<=100){
    Serial.println("    Muy iluminado");
  }
  return iluminacion;
}

//---------------------------------------------------------------------------------------------------------------------------------------
// pausar()
//---------------------------------------------------------------------------------------------------------------------------------------

void pausar(int pausa){
  char data=0;
  if(pausa==0){
    delay(1000); //Esperamos 1s
    Serial.println("  Pulse el número 1 para continuar...");
  }
  while(pausa!=1){
    delay(100);
    data = 0;
    data = Serial.read();
      if (data == '1'){
        pausa = 1;
      }
   }
}

//---------------------------------------------------------------------------------------------------------------------------------------
// sleep()
//---------------------------------------------------------------------------------------------------------------------------------------

  int sleep(int cont2,int sleepTimeS){
    if (cont2<10){
      cont2++;
    }
    else{
      Serial.println("  ESP8266 esta en modo sleep");
      ESP.deepSleep(sleepTimeS * 1000000);
    }
    return cont2;
  }

//---------------------------------------------------------------------------------------------------------------------------------------
// media()
//---------------------------------------------------------------------------------------------------------------------------------------

void media(int cont2, int humedad, int * mediaHumedad, int salinidad, int *mediaSalinidad, float temperatura, float *mediaTemperatura, int iluminacion, int *mediaIluminacion){  
  if(cont2==1){
    *mediaHumedad=0;
    *mediaSalinidad=0;
    *mediaTemperatura=0;
    *mediaIluminacion=0;
  }
  if(cont2<=10){
    *mediaHumedad=*mediaHumedad+humedad;
    *mediaSalinidad=*mediaSalinidad+salinidad;
    *mediaTemperatura=*mediaTemperatura+temperatura;
    *mediaIluminacion=*mediaIluminacion+iluminacion;
  }
  if(cont2==10){
    *mediaHumedad=*mediaHumedad/10;
    *mediaSalinidad=*mediaSalinidad/10;
    *mediaTemperatura=*mediaTemperatura/10;
    *mediaIluminacion=*mediaIluminacion/10;
    delay(1000); //Esperamos 1s
    Serial.print("  La media de la humedad es de: ");
    Serial.print(*mediaHumedad);
    Serial.println("%");
    Serial.print("  La media de la salinidad es de: ");
    Serial.print(*mediaSalinidad);
    Serial.println("%");
    Serial.print("  La media de la iluminacion es de: ");
    Serial.print(*mediaIluminacion);
    Serial.println("%");
    Serial.print("  La media de la temperatura es de: ");
    Serial.print(*mediaTemperatura);
    Serial.println(" ºC(Grados Celsius)");
    Serial.println("");
  }
}

//---------------------------------------------------------------------------------------------------------------------------------------
// alertaSalinidad()
//---------------------------------------------------------------------------------------------------------------------------------------

bool alertaSalinidad(int mediaSalinidad){
  bool alerta;
  if(mediaSalinidad<30){
    alerta=false;
  }
  if(mediaSalinidad==30){
    Serial.println("  ALERTA La salinidad es del 30%");
    alerta=true;
  }
  if(mediaSalinidad>30 && mediaSalinidad<40){
    Serial.println("  ALERTA La salinidad ha superado el 30%");
    alerta=true;
  }
  if(mediaSalinidad==40){
    Serial.println("  ALERTA La salinidad es del 40%");
    alerta=true;
  }
  if(mediaSalinidad>40 && mediaSalinidad<50){
    Serial.println("  ALERTA La salinidad ha superado el 40%");
    alerta=true;
  }
  if(mediaSalinidad==50){
    Serial.println("  ALERTA La salinidad es del 50%");
    alerta=true;
  }
  if(mediaSalinidad>50 && mediaSalinidad<60){
    Serial.println("  ALERTA La salinidad ha superado el 50%");
    alerta=true;
  }
  if(mediaSalinidad==60){
    Serial.println("  ALERTA La salinidad es del 60%");
    alerta=true;
  }
  if(mediaSalinidad>60 && mediaSalinidad<70){
    Serial.println("  ALERTA La salinidad ha superado el 60%");
    alerta=true;
  }
  if(mediaSalinidad==70){
    Serial.println("  ALERTA La salinidad es del 70%");
    alerta=true;
  }
  if(mediaSalinidad>70 && mediaSalinidad<80){
    Serial.println("  ALERTA La salinidad ha superado el 70%");
    alerta=true;
  }
  if(mediaSalinidad==80){
    Serial.println("  ALERTA La salinidad es del 80%");
    alerta=true;
  }
  if(mediaSalinidad>80 && mediaSalinidad<90){
    Serial.println("  ALERTA La salinidad ha superado el 80%");
    alerta=true;
  }
  if(mediaSalinidad==90){
    Serial.println("  ALERTA La salinidad es del 90%");
    alerta=true;
  }
  if(mediaSalinidad>90 && mediaSalinidad<100){
    Serial.println("  ALERTA La salinidad ha superado el 90%");
    alerta=true;
  }
  if(mediaSalinidad==100){
    Serial.println("  ALERTA La salinidad es del 100%");
    alerta=true;
  }
  Serial.println("");
  return alerta;
}
  
