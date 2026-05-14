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

// Actuadores / Relés / LEDs
const int pinMantaTermica = 6;   // ROJO
const int pinHumidificador = 7;  // AZUL
const int pinCooler = 8;         // VERDE
const int pinLuces = 9;          // AMARILLO / BLANCO

// ======================================================
// INSTANCIAS
// ======================================================

RTC_DS3231 rtc;
SHT2x sht;

bool sdPresente = false;

// ======================================================
// SETUP
// ======================================================

void setup() {

  Serial.begin(9600);

  // ---------------- I2C ----------------

  Wire.begin();
  Wire.setClock(100000);

  // ======================================================
  // CONFIGURACIÓN DE PINES
  // ======================================================

  pinMode(pinMantaTermica, OUTPUT);
  pinMode(pinHumidificador, OUTPUT);
  pinMode(pinCooler, OUTPUT);
  pinMode(pinLuces, OUTPUT);

  // RELÉS ACTIVOS EN LOW
  // HIGH = apagado
  // LOW  = encendido

  digitalWrite(pinMantaTermica, HIGH);
  digitalWrite(pinHumidificador, HIGH);
  digitalWrite(pinCooler, HIGH);
  digitalWrite(pinLuces, HIGH);

  // ======================================================
  // INICIALIZACIÓN SD
  // ======================================================

  Serial.print("Iniciando SD...");

  if (!SD.begin(SD_CS)) {

    Serial.println(" [!] No se encontro tarjeta SD.");
    sdPresente = false;

  } else {

    Serial.println(" [OK] Tarjeta SD lista.");
    sdPresente = true;
  }

  // ======================================================
  // INICIALIZACIÓN RTC
  // ======================================================

  if (!rtc.begin()) {

    Serial.println(" [!] No se encontro RTC.");

  } else {

    Serial.println(" [OK] RTC detectado.");

    // SINCRONIZAR SOLO UNA VEZ
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // ======================================================
  // INICIALIZACIÓN SENSOR SHT
  // ======================================================

  if (!sht.begin()) {

    Serial.println(" [!] No se encontro SHT2x.");

  } else {

    Serial.println(" [OK] Sensor SHT2x detectado.");
  }

  Serial.println("-----------------------------------");
}

// ======================================================
// LOOP
// ======================================================

void loop() {

  // ======================================================
  // LECTURA SENSOR
  // ======================================================

  sht.read();

  float temp = sht.getTemperature();
  float hum = sht.getHumidity();

  // ======================================================
  // LECTURA RTC
  // ======================================================

  DateTime now = rtc.now();

  // ======================================================
  // MONITOR SERIAL
  // ======================================================

  Serial.print(now.day());
  Serial.print("/");

  Serial.print(now.month());
  Serial.print("/");

  Serial.print(now.year());

  Serial.print(" ");

  if (now.hour() < 10) Serial.print("0");
  Serial.print(now.hour());

  Serial.print(":");

  if (now.minute() < 10) Serial.print("0");
  Serial.print(now.minute());

  Serial.print(":");

  if (now.second() < 10) Serial.print("0");
  Serial.print(now.second());

  Serial.print(" -> Temp: ");

  Serial.print(temp, 1);

  Serial.print(" C | Hum: ");

  Serial.print(hum, 1);

  Serial.println(" %");

  // ======================================================
  // GUARDAR EN SD
  // ======================================================

  if (sdPresente) {

    File dataFile = SD.open("log.txt", FILE_WRITE);

    if (dataFile) {

      // Encabezado automático
      if (dataFile.size() == 0) {

        dataFile.println("FechaHora,Temperatura,Humedad");
      }

      dataFile.print(now.timestamp());
      dataFile.print(",");

      dataFile.print(temp);
      dataFile.print(",");

      dataFile.println(hum);

      dataFile.close();

    } else {

      Serial.println(" [!] ERROR SD: No se puede escribir.");
      sdPresente = false;
    }
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

  // Ejemplo: siempre encendidas

  digitalWrite(pinLuces, LOW);

  // ======================================================
  // TIEMPO DE MUESTREO
  // ======================================================

  delay(5000);
}
