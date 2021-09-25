/*
  Autor: Vidal Bazurto (avbazurt@espol.edu.ec)
  Sensor PZEM004T
*/

#include "MC_Sensor.h"

#define RX2 16
#define TX2 17

Medidor_Consumo::Medidor_Consumo():
  _faseA(Serial2, RX2, TX2, 0x20),
  _faseB(Serial2, RX2, TX2, 0x05)
{
  SensorFaseA = {0, 0, 0, 0, 0, 0};
  SensorFaseB = {0, 0, 0, 0, 0, 0};

}

void Medidor_Consumo::begin(String tipo, ESP32Time rtc) {
  type = tipo;
  MAC = WiFi.macAddress();
  RTC = rtc;
}

void Medidor_Consumo::reset(String comando) {
  if (comando == "ALL") {
    Serial.println("Reset Bifasico");
    _faseA.resetEnergy(); 
    if (type == "BIFASICO") {
      _faseB.resetEnergy();
    }
  }
  else if (comando == "S1") {
    Serial.println("Reset S1");
    _faseA.resetEnergy();
  }

  else if (comando == "S2") {
    Serial.println("Reset S2");
    if (type == "BIFASICO") {
      _faseB.resetEnergy();
    }
  }
}


void Medidor_Consumo::address(PZEM004Tv30 sensor){
    Serial.print("PZEM ");
    Serial.print("Address:");
    Serial.println(sensor.getAddress(), HEX);
    Serial.println("===================");
}

void Medidor_Consumo::medicion(void) {
  address(_faseA);
  SensorFaseA.voltaje = _faseA.voltage();
  SensorFaseA.corriente = _faseA.current();
  SensorFaseA.potencia = _faseA.power();
  SensorFaseA.frecuencia = _faseA.frequency();
  SensorFaseA.energia = _faseA.energy();
  SensorFaseA.FP = _faseA.pf();

  if (isnan(SensorFaseA.voltaje)) {
    SensorFaseA.voltaje = -1;
  }
  if (isnan(SensorFaseA.corriente)) {
    SensorFaseA.corriente = -1;
  }
  if (isnan(SensorFaseA.potencia)) {
    SensorFaseA.potencia = -1;
  }
  if (isnan(SensorFaseA.frecuencia)) {
    SensorFaseA.frecuencia = -1;
  }
  if (isnan(SensorFaseA.energia)) {
    SensorFaseA.energia = -1;
  }
  if (isnan(SensorFaseA.FP)) {
    SensorFaseA.FP = -1;
  }

  if (type == "BIFASICO") {
    address(_faseB);
    SensorFaseB.voltaje = _faseB.voltage();
    SensorFaseB.corriente = _faseB.current();
    SensorFaseB.potencia = _faseB.power();
    SensorFaseB.frecuencia = _faseB.frequency();
    SensorFaseB.energia = _faseB.energy();
    SensorFaseB.FP = _faseB.pf();

    if (isnan(SensorFaseB.voltaje)) {
      SensorFaseB.voltaje = -1;
    }
    if (isnan(SensorFaseB.corriente)) {
      SensorFaseB.corriente = -1;
    }
    if (isnan(SensorFaseB.potencia)) {
      SensorFaseB.potencia = -1;
    }
    if (isnan(SensorFaseB.frecuencia)) {
      SensorFaseB.frecuencia = -1;
    }
    if (isnan(SensorFaseB.energia)) {
      SensorFaseB.energia = -1;
    }
    if (isnan(SensorFaseB.FP)) {
      SensorFaseB.FP = -1;
    }
  }
}


void Medidor_Consumo::updateJson(void) {
  doc.clear();
  doc["modelo"] = "MC_" + type;
  sensores = doc.createNestedArray("sensores");

  sensores_0 = sensores.createNestedObject();
  sensores_1 = sensores.createNestedObject();
  sensores_2 = sensores.createNestedObject();
  sensores_3 = sensores.createNestedObject();
  sensores_4 = sensores.createNestedObject();
  sensores_5 = sensores.createNestedObject();

  //----------VOLTAJE-------------
  sensores_0["nombre"] = "VA";
  sensores_0["valor"] = String(SensorFaseA.voltaje);
  sensores_0["unidadMedicion"] = "V";

  //----------CORRIENTE-------------
  sensores_1["nombre"] = "IA";
  sensores_1["valor"] = String(SensorFaseA.corriente);
  sensores_1["unidadMedicion"] = "A";

  //----------POTENCIA-------------
  sensores_2["nombre"] = "PA";
  sensores_2["valor"] = String(SensorFaseA.potencia);
  sensores_2["unidadMedicion"] = "W";

  //----------FRECUENCIA-------------
  sensores_3["nombre"] = "FA";
  sensores_3["valor"] = String(SensorFaseA.frecuencia);
  sensores_3["unidadMedicion"] = "Hz";

  //----------ENERGIA-------------
  sensores_4["nombre"] = "EA";
  sensores_4["valor"] = String(SensorFaseA.energia);
  sensores_4["unidadMedicion"] = "KwH";

  //----------FP-------------
  sensores_5["nombre"] = "FPA";
  sensores_5["valor"] = String(SensorFaseA.FP);
  sensores_5["unidadMedicion"] = "";


  if (type == "BIFASICO") {
    sensores_6 = sensores.createNestedObject();
    sensores_7 = sensores.createNestedObject();
    sensores_8 = sensores.createNestedObject();
    sensores_9 = sensores.createNestedObject();
    sensores_10 = sensores.createNestedObject();
    sensores_11 = sensores.createNestedObject();

    //----------VOLTAJE-------------
    sensores_6["nombre"] = "VB";
    sensores_6["valor"] = String(SensorFaseB.voltaje);
    sensores_6["unidadMedicion"] = "V";

    //----------CORRIENTE-------------
    sensores_7["nombre"] = "IB";
    sensores_7["valor"] = String(SensorFaseB.corriente);
    sensores_7["unidadMedicion"] = "A";

    //----------POTENCIA-------------
    sensores_8["nombre"] = "PB";
    sensores_8["valor"] = String(SensorFaseB.potencia);
    sensores_8["unidadMedicion"] = "W";

    //----------FRECUENCIA-------------
    sensores_9["nombre"] = "FB";
    sensores_9["valor"] = String(SensorFaseB.frecuencia);
    sensores_9["unidadMedicion"] = "Hz";

    //----------ENERGIA-------------
    sensores_10["nombre"] = "EB";
    sensores_10["valor"] = String(SensorFaseB.energia);
    sensores_10["unidadMedicion"] = "KwH";

    //----------FP-------------
    sensores_11["nombre"] = "FPB";
    sensores_11["valor"] = String(SensorFaseB.FP);
    sensores_11["unidadMedicion"] = "";
  }
  
  temperatura = sensores.createNestedObject();
  temperatura["nombre"] = "TEMP";
  temperatura["valor"] = String((temprature_sens_read() - 32) / 1.8);
  temperatura["unidadMedicion"] = "C";

  text_Json = "";
  serializeJson(doc, text_Json);
}


String Medidor_Consumo::generateString(void) {
  //Actualizo datos del json
  FECHA_HORA = RTC.getTime("#%Y-%m-%d#%H:%M:%S#");
  medicion();
  updateJson();
  return MAC + FECHA_HORA + text_Json;
}
