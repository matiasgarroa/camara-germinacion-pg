#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
#include "SHT2x.h"

// ======================================================
// CONFIGURACIÓN DE PINES
// ======================================================

// SD
const int SD_CS = 10;

// Actuadores (LEDs simulados)
const int pinMantaTermica = 6;   // ROJO
const int pinHumidificador = 7;  // AZUL
const int pinCooler = 8;         // VERDE
const int pinLuces = 9;          // AMARILLO / BLANCO

// ======================================================
// INSTANCIAS DE MÓDULOS
// ======================================================
RTC_DS3231 rtc;
SHT2x sht;
bool sdPresente = false;

// ======================================================
// SETUP
// ======================================================
void setup() {
  Serial.begin(9600);
  Wire.begin(); // Bus I2C para RTC y SHT. Sin setClock para evitar problemas en placas WAVGAT

  // --- CONFIGURACIÓN DE PINES DE ACTUADORES ---
  pinMode(pinMantaTermica, OUTPUT);
  pinMode(pinHumidificador, OUTPUT);
  pinMode(pinCooler, OUTPUT);
  pinMode(pinLuces, OUTPUT);

  // Estado inicial
  digitalWrite(pinMantaTermica, HIGH);
  digitalWrite(pinHumidificador, HIGH);
  digitalWrite(pinCooler, HIGH);
  digitalWrite(pinLuces, HIGH);

  // --- MÓDULO 1: INICIALIZACIÓN SD ---
  Serial.print("Iniciando SD...");
  if (!SD.begin(SD_CS)) {
    Serial.println(" [!] No se encontro tarjeta. Continuando sin SD.");
    sdPresente = false;
  } else {
    Serial.println(" [OK] Tarjeta lista.");
    sdPresente = true;
  }

  // --- MÓDULO 2: INICIALIZACIÓN RTC ---
  if (!rtc.begin()) {
    Serial.println(" [!] No se encontro RTC.");
  } else if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // --- MÓDULO 3: INICIALIZACIÓN SHT2x ---
  if (!sht.begin()) {
    Serial.println(" [!] No se encontro SHT2x.");
  }
  
  Serial.println("-----------------------------------");
  
  if (sdPresente) {
    File dataFile = SD.open("log.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println("=== SISTEMA ENCENDIDO / REINICIADO ===");
      dataFile.close();
    }
  }
}

// ======================================================
// LOOP
// ======================================================
void loop() {
  // --- LECTURA SENSOR SHT ---
  sht.read();
  float temp = sht.getTemperature();
  float hum = sht.getHumidity();

  // --- LECTURA TIEMPO RTC ---
  DateTime now = rtc.now();

  // --- SALIDA DE DATOS POR SERIAL ---
  Serial.print(now.day()); Serial.print("/"); Serial.print(now.month()); Serial.print(" ");
  
  // Agregamos formato con ceros a la hora para que se vea ordenado (ej: 09:05:02)
  if (now.hour() < 10) Serial.print("0");
  Serial.print(now.hour()); Serial.print(":");
  if (now.minute() < 10) Serial.print("0");
  Serial.print(now.minute()); Serial.print(":");
  if (now.second() < 10) Serial.print("0");
  Serial.print(now.second());
  
  Serial.print(" -> Temp: "); Serial.print(temp, 1);
  Serial.print("C | Hum: "); Serial.print(hum, 1); Serial.println("%");

  // --- MÓDULO 4: REGISTRO EN SD ---
  File dataFile = SD.open("log.txt", FILE_WRITE);
  
  if (dataFile) {
    // Si el archivo está vacío, le agrega el encabezado
    if (dataFile.size() == 0) {
      dataFile.println("FechaHora,Temperatura,Humedad");
    }
    
    dataFile.print(now.timestamp());
    dataFile.print(","); dataFile.print(temp);
    dataFile.print(","); dataFile.println(hum);
    dataFile.close();
    
    if (!sdPresente) {
      Serial.println(" [!] Tarjeta insertada y detectada.");
      sdPresente = true;
    }
  } else {
    Serial.println(" [!] ERROR SD: No se puede escribir. Verifique que la tarjeta este insertada.");
    sdPresente = false;
  }

  // ======================================================
  // CONTROL HUMEDAD
  // ======================================================
  // AZUL = Humidificador
  if (hum > 40) {
    digitalWrite(pinHumidificador, LOW);
    Serial.println("Humedad normal -> Humidificador APAGADO");
  } else {
    digitalWrite(pinHumidificador, HIGH);
    Serial.println("Humedad baja -> Humidificador ENCENDIDO");
  }

  // ======================================================
  // CONTROL TEMPERATURA
  // ======================================================
  // ROJO = Manta térmica
  if (temp < 25) {
    digitalWrite(pinMantaTermica, HIGH);
    Serial.println("Temperatura baja -> Manta TERMICA ENCENDIDA");
  } else {
    digitalWrite(pinMantaTermica, LOW);
    Serial.println("Temperatura normal -> Manta TERMICA APAGADA");
  }

  // VERDE = Cooler
  if (temp > 30) {
    digitalWrite(pinCooler, HIGH);
    Serial.println("Temperatura alta -> COOLER ENCENDIDO");
  } else {
    digitalWrite(pinCooler, LOW);
    Serial.println("Temperatura normal -> COOLER APAGADO");
  }

  // ======================================================
  // LUCES
  // ======================================================
  // Siempre encendidas en esta simulación
  digitalWrite(pinLuces, LOW);

  Serial.println("-----------------------------------");
  
  // Tiempo de muestreo
  delay(5000); 
}
