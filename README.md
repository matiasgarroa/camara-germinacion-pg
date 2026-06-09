# 🌱 Cámara de Germinación Automatizada - Cálculo de Poder Germinativo (PG)

Este repositorio contiene la documentación, diseño y código fuente para la construcción de una cámara de germinación controlada. El proyecto está orientado específicamente a la estandarización y automatización de los ensayos para el cálculo del **Poder Germinativo (PG)** en semillas agronómicas.

## 👥 Integrantes
* Agustín Bascuñan
* Santiago Boato
* Matias Garro Alou

## 🚀 ¿Para qué sirve?
El sistema sirve para ejecutar ensayos de germinación en un microclima donde las variables ambientales (temperatura, humedad y fotoperiodo) son manipuladas a voluntad de forma automática. Esto permite evaluar la respuesta fisiológica de las semillas en condiciones óptimas e ininterrumpidas, garantizando que los resultados dependan exclusivamente del vigor biológico del lote y no del clima del entorno.

## 🎯 ¿Qué problemas se buscan abordar?
El ensayo de Poder Germinativo es fundamental para determinar la viabilidad de un lote de semillas. Los métodos manuales convencionales están expuestos a fluctuaciones ambientales que pueden alterar los resultados. 
Este proyecto busca solucionar:
* **Falta de estabilidad climática:** Mantiene la temperatura y humedad en los rangos estrictos que requiere la semilla de forma automatizada.
* **Dependencia manual:** Automatiza los ciclos de luz, ventilación y mantenimiento de humedad sin necesidad de intervención humana constante.
* **Carencia de trazabilidad de datos:** Elimina la pérdida de información sobre lo que ocurre durante la noche o los fines de semana.

## 📊 ¿Para qué sirve el registro de datos?
El sistema de registro continuo (Datalogger) guarda un historial ininterrumpido en la memoria MicroSD. Esto es crucial para la **trazabilidad y validez del ensayo**. Permite:
* Demostrar de forma objetiva que el ensayo cumplió con los estándares de temperatura y humedad durante los 7 a 14 días requeridos.
* Graficar las curvas climáticas y cruzarlas con la curva de emergencia de las plántulas.
* Descartar fallas de equipamiento: si el Poder Germinativo resulta bajo, los datos permiten comprobar que no fue a causa de un golpe de calor o falta de humedad en la cámara.

## 🧑‍🌾 ¿Quién es el público objetivo?
* Estudiantes universitarios de ciencias agrarias y botánica.
* Productores agrícolas que necesiten testear la calidad de su material de siembra antes de ir al lote.
* Laboratorios comerciales de análisis de semillas y viveristas.
* Investigadores enfocados en fisiología vegetal y biotecnología.

## 🛠️ ¿Qué materiales se necesitan?

**Electrónica de Control y Registro**
* Placa Arduino UNO R3 o Arduino NANO
* Sensor SHT31 (Protocolo I2C para alta precisión de temperatura/humedad)
* Módulo RTC DS3231 (Reloj de tiempo real)
* Módulo Lector de MicroSD (SPI) + Tarjeta MicroSD

**Actuadores y Climatización**
* Módulo de Relés de 4 Canales (Optoacoplado de 5V) o transistores independientes.
* Manta Térmica para Terrarios (15W a 20W)
* Módulo Humidificador Ultrasónico de 5V
* Cooler de PC de 12V (80mm o 120mm)
* Tira LED Full Spectrum (Opcional, recomendada)

**Alimentación y Conexiones**
* Fuente de Alimentación de 12V (2 o 3 Amperes)
* Módulo Step-Down LM2596
* Conector Jack Hembra o Bornera
* Protoboard de 400 u 830 puntos y cables Jumper

**Estructura y Sustrato de Ensayo**
* Conservadora de telgopor (30 a 50 litros)
* Tupper o Caja Estanca Pequeña (para guardar y aislar el Arduino de la humedad)
* Papel de Germinación (Tipo Kraft o secante grueso)
* Bandeja de plástico con rejilla

## 💻 ¿Necesita algún software propio?
La programación del microcontrolador se realiza a través del **Arduino IDE**. El sistema requiere la instalación de librerías de terceros para la comunicación con el sensor SHT31, el reloj RTC y el módulo de memoria SD. Los archivos de registro generados (.csv o .txt) podrán ser analizados con cualquier software de hojas de cálculo convencional (como Excel o Google Sheets).

## 📍 ¿Dónde se va a probar?
Las pruebas de estrés germinativo y calibración de los sensores se llevarán a cabo íntegramente en el interior de la **conservadora de telgopor**. Este habitáculo funcionará como cámara de aislamiento térmico, albergando la bandeja de plástico y el papel secante, garantizando un microclima estable sin interferencias del ambiente exterior.

## 🏫 ¿Cómo lo mostraremos en el aula?
Se presentará el hardware ensamblado, mostrando la separación entre la etapa de control (Arduino aislado) y la etapa de climatización dentro de la conservadora.
Realizaremos una demostración en vivo modificando las variables ambientales (acercando calor o humedad al sensor) para que se observe cómo el sistema toma decisiones y activa las salidas correspondientes. Además, extraeremos la tarjeta MicroSD en tiempo real y abriremos el archivo generado en una computadora para evidenciar la capacidad de guardado y trazabilidad del datalogger.

## 📈 Estado Actual y Avances

* **Semana 1:** Se completó la etapa de investigación técnica, la definición del alcance agronómico, la selección de los requerimientos de hardware y el diseño del flujo lógico del sistema.
* **Semana 2:** Se finalizó la lógica del código principal. Para avanzar con las pruebas, se realizó el diseño eléctrico en la protoboard simulando las cargas con luces LED, lo que permitió validar visualmente los momentos exactos de conmutación antes de utilizar los relés definitivos.
* **Semana 3:** Se trabajó en el hardware definitivo, verificando que todos los componentes pudieran ser integrados correctamente. Se depuraron y solucionaron los problemas de sincronización del reloj RTC y los errores de escritura en el módulo de la tarjeta SD, garantizando un guardado de datos estable.
Arquitectura de las mejoras aplicadas
* **Semana 4:** Arquitectura de las mejoras aplicadas:
  - Multitarea Real (Fin del delay): Se implementó la función millis(). Usar delay(5000) detenía por completo el procesador. Con millis(), el Arduino verifica constantemente si es momento de ejecutar una tarea, permitiendo agregar futuras funciones (como botones o pantallas) sin lag.
  - Protección de la Tarjeta SD (Wear Leveling): Escribir en la memoria flash cada 5 segundos iba a destruir la tarjeta SD en semanas y ralentizaba el bucle. Se desacoplaron los tiempos: el clima se controla cada 5 segundos, pero la SD guarda datos cada 1 minuto (configurable en INTERVALO_SD).
  - Histéresis (Cuidado de Relés): En el código original, si la humedad oscilaba rápidamente entre 39.9% y 40.1%, el relé se prendía y apagaba frenéticamente, acortando su vida útil. Se agregó la constante HISTERESIS. Ahora el humidificador enciende si cae a 38% y solo se apaga al tocar el 40%.
  - Optimización de Memoria (SRAM): Todo texto estático enviado por Serial (ej. "Temperatura alta") se envolvió en la macro F(). Esto traslada los textos de la frágil memoria SRAM (donde corre el código vivo) a la memoria Flash, previniendo reinicios súbitos por desbordamiento de memoria.
  - Velocidad de Bus y Formateo: Se subió el baud rate de 9600 a 115200 para que imprimir en pantalla no ralentice el bucle. Además, se reemplazaron las múltiples líneas Serial.print de la fecha/hora por un sprintf, que arma una cadena limpia e imprime todo en una sola operación de CPU.

## 🚧 Alcance Actual, Limitaciones y Mejoras Futuras

**Alcance y Limitaciones Actuales:**
* **Prototipo en fase de simulación de potencia:** Actualmente, debido a la disponibilidad de componentes, los actuadores físicos definitivos (módulo de relés, manta térmica, humidificador ultrasónico) están siendo simulados mediante un arreglo de luces LED estándar en la protoboard. Esto nos permitió validar al 100% la lógica del microcontrolador, la multitarea no bloqueante y la lectura de sensores de forma segura.
* **Lectura de datos local:** El sistema almacena el historial climático de forma local en la tarjeta SD, por lo que requiere la extracción física de la memoria para su análisis posterior.

**Mejoras Necesarias / Potenciales:**
* Integrar físicamente los actuadores de potencia de 12V/220V dentro de la conservadora de telgopor utilizando el módulo de relés optoacoplado.
* Migrar en un futuro a una placa con conectividad Wi-Fi (como el ESP32) para transmitir las variables agronómicas y alertar sobre desvíos climáticos directamente a un teléfono móvil o dashboard en la nube.

## 💾 Firmware y Código Fuente
El código principal del microcontrolador (Firmware) se encuentra alojado en este repositorio en el archivo correspondiente `.ino`. 
Para su compilación, se requiere la instalación de las siguientes librerías de terceros:
* `RTClib.h` (Para el manejo del tiempo real)
* `SHT2x.h` o equivalente (Para la lectura del sensor de temperatura y humedad)
* Las librerías nativas `Wire.h`, `SPI.h` y `SD.h`.

## 🔌 Esquemático del Circuito
A continuación se detalla el diagrama de conexiones entre el Arduino, el bus I2C (sensores), el bus SPI (Módulo SD) y las salidas digitales para el control de los relés/LEDs.


![Esquemático del Circuito](ruta-a-tu-imagen-del-esquematico.jpg)

## 📦 Diseño del Prototipo Final
En el siguiente esquema se ilustra la disposición final de los componentes. El sistema consta de dos zonas aisladas: una caja estanca exterior (donde reside el Arduino y la electrónica de control para evitar cortocircuitos por humedad) y el interior de la conservadora de telgopor (donde se ubican las semillas, los actuadores climáticos y las sondas de los sensores).

![Esquema de la cámara](ruta-a-tu-dibujo-o-foto.jpg)
