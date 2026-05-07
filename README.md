# Cámara de Germinación Automatizada - Cálculo de Poder Germinativo (PG)

Este repositorio contiene la documentación, diseño y código fuente para la construcción de una cámara de germinación controlada. El proyecto está orientado específicamente a la estandarización y automatización de los ensayos para el cálculo del **Poder Germinativo (PG)** en semillas agronómicas.

## 👥 Integrantes
* Agustín Bescuñan
* Santiago Boato
* Matias Garro Alou

## 🎯 ¿Qué problemas se buscan abordar?
El ensayo de Poder Germinativo es fundamental para determinar la viabilidad y el vigor de un lote de semillas. Los métodos manuales convencionales están expuestos a fluctuaciones ambientales que pueden alterar los resultados. 
Este proyecto busca solucionar:
* **Falta de estabilidad climática:** Mantiene la temperatura y humedad en los rangos estrictos que requiere la semilla de forma automatizada.
* **Carencia de trazabilidad de datos:** Implementa un sistema de registro continuo (Datalogging) para analizar el historial de temperatura y humedad durante todo el ciclo del ensayo.
* **Dependencia manual:** Automatiza los ciclos de luz, ventilación y mantenimiento de humedad sin necesidad de intervención humana constante.

## 🛠️ ¿Qué materiales se necesitan?

**Electrónica de Control y Registro**
* Placa Arduino UNO R3 o Arduino NANO
* Sensor SHT31 (Protocolo I2C para alta precisión de temperatura/humedad)
* Módulo RTC DS3231 (Reloj de tiempo real)
* Módulo Lector de MicroSD (SPI) + Tarjeta MicroSD

**Actuadores y Climatización**
* Módulo de Relés de 4 Canales (Optoacoplado de 5V)
* Manta Térmica para Terrarios (15W a 20W)
* Módulo Humidificador Ultrasónico de 5V
* Cooler de PC de 12V (80mm o 120mm)
* Tira LED Full Spectrum (Opcional, recomendada)

**Alimentación y Conexiones**
* Fuente de Alimentación de 12V (2 o 3 Amperes)
* Módulo Step-Down LM2596
* Conector Jack Hembra o Bornera
* Protoboard de 400 u 830 puntos (para armar sin soldar)
* Cables Jumper

**Estructura y Sustrato de Ensayo**
* Conservadora de telgopor (30 a 50 litros)
* Tupper o Caja Estanca Pequeña (para guardar y aislar el Arduino)
* Papel de Germinación (Tipo Kraft o secante grueso)
* Bandeja de plástico con rejilla

## 💻 ¿Necesita algún software propio?
La programación del microcontrolador se realiza a través del **Arduino IDE**. El sistema requiere la instalación de librerías de terceros para la comunicación con el sensor SHT31, el reloj RTC y el módulo de memoria SD. Posteriormente, los archivos de registro generados por la cámara (archivos .csv o .txt) podrán ser analizados con cualquier software de hojas de cálculo convencional (como Excel o Google Sheets) para trazar las curvas de las variables ambientales.

## 📍 ¿Dónde se va a probar?
Las pruebas de estrés germinativo y calibración de los sensores se llevarán a cabo íntegramente en el interior de la **conservadora de telgopor**. Este habitáculo funcionará como la cámara de aislamiento térmico, albergando la bandeja de plástico y el papel secante, garantizando así un microclima estable sin interferencias del ambiente exterior.

## 📈 Estado Actual y Avances
* **Día 1:** Se completó la etapa de investigación técnica, la selección de los requerimientos de hardware y el diseño del flujo del sistema.
* **Lógica del Código:** Toda la lógica de control, la lectura de los sensores y la estructura de toma de decisiones del código están listas. Para poder avanzar con las pruebas de software de manera inmediata, el diseño electrónico se adaptó para realizar una simulación en la protoboard usando luces LED normales en lugar de los actuadores definitivos. Esto permite validar visualmente los momentos exactos de conmutación antes de integrar físicamente el módulo de relés de 4 canales.
