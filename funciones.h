#ifndef TODAS_LAS_FUNCIONES
#define TODAS_LAS_FUNCIONES

  int inicio(int inicioC);
  
  void calibrarHumedad1(int16_t adc1, int *AirValue, int *WaterValue);
  void calibrarHumedad2(int16_t adc1, int *AirValue, int *WaterValue);
  void calibrarHumedadAutomatica(int16_t adc1, int *AirValue, int *WaterValue);
  
  void calibrarSalinidad1(int adc0,int * SalValue,int * NoSalValue);
  void calibrarSalinidad2(int adc0,int * SalValue,int * NoSalValue);
  void calibrarSalinidadAutomatica(int adc0,int * SalValue,int * NoSalValue);

  bool alertaSalinidad(int salinidad);
  
  void calibrarIluminacion(int16_t adc3,int * LightValue);
  void calibrarIluminacionAutomatica(int16_t adc3,int * LightValue);
  
  int leerHumedad(int AirValue,int WaterValue, int16_t adc1);
  int leerSalinidad(int SalValue,int NoSalValue,int16_t adc0);
  int leerTemperatura(int16_t adc2, float b, float m);
  int leerIluminacion(int16_t adc3,int LightValue);

  void pausar(int pausa);
  int sleep(int cont2,int sleepTimeS);
  void media(int cont2,int humedad,int *mediaHumedad, int salinidad, int *mediaSalinidad, float temperatura, float *mediaTemperatura,int iluminacion,int *mediaIluminacion);

  #endif
