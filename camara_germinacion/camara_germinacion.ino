#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
#include "SHT2x.h"

// --- CONFIGURACIÓN DE PINES ---
const int SD_CS = 10; // Pin Digital para Chip Select del SD (Protocolo SPI)

// 1. DEFINICIÓN DE PINES (Simulación de Actuadores)
// Le asignamos un nombre a cada pin para que el código sea fácil de leer
const int pinMantaTermica = 6;  // LED Rojo
const int pinHumidificador = 7; // LED Azul
const int pinCooler = 8;        // LED Verde
const int pinLuces = 9;         // LED Amarillo o Blanco

// --- INSTANCIAS DE MÓDULOS ---
RTC_DS3231 rtc;
SHT2x sht;
bool sdPresente = false; // Variable para rastrear el estado de la SD

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Bus I2C para RTC y SHT (Pines A4/A5 en WAVGAT)

  // 2. CONFIGURACIÓN INICIAL DE ACTUADORES
  // Le decimos al Arduino que estos pines van a emitir energía (OUTPUT)
  pinMode(pinMantaTermica, OUTPUT);
  pinMode(pinHumidificador, OUTPUT);
  pinMode(pinCooler, OUTPUT);
  pinMode(pinLuces, OUTPUT);

  // --- MÓDULO 1: INICIALIZACIÓN SD ---
  Serial.print("Iniciando SD...");
  if (!SD.begin(SD_CS)) {
    Serial.println(" [!] No se encontro tarjeta. Continuando sin SD.");
    sdPresente = false;
  } else {
    Serial.println(" [OK] Tarjeta lista.");
    sdPresente = true;
  }

  // --- MÓDULO 2: INICIALIZACIÓN RTC (Digital) ---
  if (!rtc.begin()) {
    Serial.println(" [!] No se encontro RTC.");
  } else if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // --- MÓDULO 3: INICIALIZACIÓN SHT2x (Digital) ---
  if (!sht.begin()) {
    Serial.println(" [!] No se encontro SHT2x.");
  }
}

void loop() {
  // --- LECTURA SENSOR SHT ---
  sht.read();
  float temp = sht.getTemperature();
  float hum = sht.getHumidity();

  // --- LECTURA TIEMPO RTC ---
  DateTime now = rtc.now();

  // --- SALIDA DE DATOS POR SERIAL ---
  Serial.print(now.day()); Serial.print("/"); Serial.print(now.month()); Serial.print(" ");
  Serial.print(now.hour()); Serial.print(":"); Serial.print(now.minute());
  Serial.print(" -> Temp: "); Serial.print(temp, 1);
  Serial.print("C | Hum: "); Serial.print(hum, 1); Serial.println("%");

  // --- MÓDULO 4: REGISTRO EN SD CON MENSAJE DE ERROR ---
  File dataFile = SD.open("log.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(now.timestamp());
    dataFile.print(","); dataFile.print(temp);
    dataFile.print(","); dataFile.println(hum);
    dataFile.close();
    if (!sdPresente) {
      Serial.println(" [!] Tarjeta insertada y detectada.");
      sdPresente = true;
    }
  } else {
    // Si no puede abrir el archivo, avisa que la tarjeta no está o falló
    Serial.println(" [!] ERROR SD: No se puede escribir. Verifique que la tarjeta este insertada.");
    sdPresente = false;
  }

  // --- 3. BUCLE PRINCIPAL (Simulación de actuadores) ---
  
  // --- Simulación Manta Térmica ---
  digitalWrite(pinMantaTermica, HIGH); // Enciende el LED Rojo
  delay(1000);                         // Espera 1000 milisegundos (1 segundo)
  digitalWrite(pinMantaTermica, LOW);  // Apaga el LED Rojo

  // --- Simulación Humidificador ---
  digitalWrite(pinHumidificador, HIGH); // Enciende el LED Azul
  delay(1000);                          
  digitalWrite(pinHumidificador, LOW);  

  // --- Simulación Cooler / Extractor ---
  digitalWrite(pinCooler, HIGH);        // Enciende el LED Verde
  delay(1000);                          
  digitalWrite(pinCooler, LOW);         

  // --- Simulación Luces (Fotoperiodo) ---
  digitalWrite(pinLuces, HIGH);         // Enciende el LED Amarillo/Blanco
  delay(1000);                          
  digitalWrite(pinLuces, LOW);          

  // Ajustado: Muestreo restante para completar 5 segundos totales (4 segs invertidos en los LEDs de arriba + 1s de pausa = 5s por ciclo)
  delay(1000); 
}
