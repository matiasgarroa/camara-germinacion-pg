#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
#include "SHT2x.h"

// ======================================================
// CONFIGURACIÓN DE PINES Y UMBRALES
// ======================================================
const int SD_CS = 10;
const int pinMantaTermica = 6;   // ROJO
const int pinHumidificador = 7;  // AZUL
const int pinCooler = 8;         // VERDE
const int pinLuces = 9;          // AMARILLO / BLANCO

// Variables agronómicas (fáciles de calibrar aquí arriba)
const float TEMP_MIN = 25.0;
const float TEMP_MAX = 30.0;
const float HUM_OBJETIVO = 40.0;
const float HISTERESIS = 2.0; // Margen para evitar rebotes mecánicos en relés

// Temporización no bloqueante (en milisegundos)
const unsigned long INTERVALO_CONTROL = 5000;  // Evalúa el clima cada 5 segundos
const unsigned long INTERVALO_SD = 60000;      // Escribe en la SD cada 1 minuto
unsigned long tiempoAnteriorControl = 0;
unsigned long tiempoAnteriorSD = 0;

// ======================================================
// INSTANCIAS GLOBALES
// ======================================================
RTC_DS3231 rtc;
SHT2x sht;
bool sdPresente = false;

// ======================================================
// SETUP
// ======================================================
void setup() {
  // Aumentamos los baudios para liberar tiempo de procesamiento
  Serial.begin(115200);
  Wire.begin(); 

  // Configuración y estado seguro inicial (APAGADOS)
  pinMode(pinMantaTermica, OUTPUT);
  pinMode(pinHumidificador, OUTPUT);
  pinMode(pinCooler, OUTPUT);
  pinMode(pinLuces, OUTPUT);

  digitalWrite(pinMantaTermica, LOW);
  digitalWrite(pinHumidificador, LOW);
  digitalWrite(pinCooler, LOW);
  digitalWrite(pinLuces, HIGH); // Simulamos luces encendidas por defecto

  Serial.println(F("=== INICIANDO SISTEMA ===")); // F() ahorra SRAM

  // INICIALIZACIÓN SD
  Serial.print(F("SD..."));
  if (!SD.begin(SD_CS)) {
    Serial.println(F(" [!] Falla. Continuando sin SD."));
  } else {
    Serial.println(F(" [OK]"));
    sdPresente = true;
    File dataFile = SD.open("log.txt", FILE_WRITE);
    if (dataFile) {
      if (dataFile.size() == 0) {
        dataFile.println(F("FechaHora,Temperatura,Humedad"));
      }
      dataFile.println(F("=== REINICIO ==="));
      dataFile.close();
    }
  }

  // INICIALIZACIÓN RTC
  Serial.print(F("RTC..."));
  if (!rtc.begin()) {
    Serial.println(F(" [!] Falla."));
  } else {
    if (rtc.lostPower()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println(F(" [OK]"));
  }

  // INICIALIZACIÓN SHT2x
  Serial.print(F("SHT2x..."));
  if (!sht.begin()) Serial.println(F(" [!] Falla."));
  else Serial.println(F(" [OK]"));
  
  Serial.println(F("-----------------------------------"));
}

// ======================================================
// LOOP PRINCIPAL (No bloqueante)
// ======================================================
void loop() {
  unsigned long tiempoActual = millis();

  // TAREA 1: CONTROL CLIMÁTICO (Cada 5 segundos)
  if (tiempoActual - tiempoAnteriorControl >= INTERVALO_CONTROL) {
    tiempoAnteriorControl = tiempoActual;
    ejecutarControlClimatico();
  }

  // TAREA 2: REGISTRO DE DATOS (Cada 1 minuto)
  if (tiempoActual - tiempoAnteriorSD >= INTERVALO_SD) {
    tiempoAnteriorSD = tiempoActual;
    registrarDatosSD();
  }
}

// ======================================================
// FUNCIONES ESPECÍFICAS
// ======================================================
void ejecutarControlClimatico() {
  sht.read();
  float temp = sht.getTemperature();
  float hum = sht.getHumidity();
  DateTime now = rtc.now();

  // Imprimir fecha y hora con formato limpio de dos dígitos (00/00 00:00:00)
  if (now.day() < 10) Serial.print('0'); 
  Serial.print(now.day());
  Serial.print('/');
  
  if (now.month() < 10) Serial.print('0'); 
  Serial.print(now.month());
  Serial.print(' ');
  
  if (now.hour() < 10) Serial.print('0'); 
  Serial.print(now.hour());
  Serial.print(':');
  
  if (now.minute() < 10) Serial.print('0'); 
  Serial.print(now.minute());
  Serial.print(':');
  
  if (now.second() < 10) Serial.print('0'); 
  Serial.print(now.second());
  
  // Imprimir los datos del clima con 1 solo decimal de forma segura
  Serial.print(F(" -> T:"));
  Serial.print(temp, 1); // El ', 1' le dice a Arduino que muestre un solo decimal
  Serial.print(F("C | H:"));
  Serial.print(hum, 1);
  Serial.println(F("%"));

  // CONTROL HUMEDAD CON HISTÉRESIS
  if (hum < (HUM_OBJETIVO - HISTERESIS)) {
    digitalWrite(pinHumidificador, HIGH); // Enciende si cae a 38%
  } else if (hum > HUM_OBJETIVO) {
    digitalWrite(pinHumidificador, LOW);  // Apaga al llegar a 40%
  }

  // CONTROL TEMPERATURA
  if (temp < TEMP_MIN) {
    digitalWrite(pinMantaTermica, HIGH);
  } else if (temp >= (TEMP_MIN + HISTERESIS)) {
    digitalWrite(pinMantaTermica, LOW);
  }

  if (temp > TEMP_MAX) {
    digitalWrite(pinCooler, HIGH);
  } else if (temp <= (TEMP_MAX - HISTERESIS)) {
    digitalWrite(pinCooler, LOW);
  }
}

void registrarDatosSD() {
  if (!sdPresente) return; // Evita bloqueos si no hay SD

  sht.read();
  File dataFile = SD.open("log.txt", FILE_WRITE);
  if (dataFile) {
    DateTime now = rtc.now();
    dataFile.print(now.timestamp());
    dataFile.print(F(",")); 
    dataFile.print(sht.getTemperature());
    dataFile.print(F(",")); 
    dataFile.println(sht.getHumidity());
    dataFile.close();
    Serial.println(F("[+] Dato guardado en SD"));
  } else {
    Serial.println(F("[!] Error escribiendo en SD"));
  }
}
