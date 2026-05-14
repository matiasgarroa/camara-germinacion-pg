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

  // --- 3. BUCLE PRINCIPAL (Encendido Secuencial de LEDs) ---
  
  // 1. Se enciende el LED Rojo (Manta) y espera 1 seg
  digitalWrite(pinMantaTermica, HIGH); 
  delay(1000);                         

  // 2. Se suma el LED Azul (Humidificador) y espera 1 seg
  digitalWrite(pinHumidificador, HIGH); 
  delay(1000);                          

  // 3. Se suma el LED Verde (Cooler) y espera 1 seg
  digitalWrite(pinCooler, HIGH);        
  delay(1000);                          

  // 4. Se suma el LED Amarillo/Blanco (Luces) y espera 1 seg
  digitalWrite(pinLuces, HIGH);         
  delay(1000);                          

  // Apagamos todos los actuadores a la vez
  digitalWrite(pinMantaTermica, LOW);
  digitalWrite(pinHumidificador, LOW);
  digitalWrite(pinCooler, LOW);
  digitalWrite(pinLuces, LOW);

  // Pausa final de 1 segundo con todo apagado antes de reiniciar el loop 
  // (Tiempo total del ciclo: 5 segundos)
  delay(1000); 
}
