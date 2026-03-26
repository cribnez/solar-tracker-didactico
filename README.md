# Estación Didáctica de Bajo Costo para Seguimiento Solar y Adquisición de Datos Meteorológicos

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Hardware: Arduino](https://img.shields.io/badge/Hardware-Arduino-00979D.svg)
![Language: C++](https://img.shields.io/badge/Language-C++-00599C.svg)
![Status: Completed](https://img.shields.io/badge/Status-Completed-success.svg)
![Location: Suchiapa, Chiapas](https://img.shields.io/badge/Location-Suchiapa%2C%20Chiapas-orange.svg)
## 📌 Descripción
Este proyecto presenta el desarrollo de un prototipo de seguidor solar monaxial de bajo costo. El sistema no solo optimiza la captación fotovoltaica mediante un control de lazo cerrado, sino que funciona como una estación de adquisición de datos (Datalogger) para variables críticas como temperatura, humedad, irradiancia y corriente generada.

## 🎯 Objetivos
- Implementar un sistema de seguimiento solar en un eje con control de lazo cerrado.
- Integrar sensores de irradiancia (LDR), temperatura/humedad (DHT11) y corriente (ACS712).
- Diseñar un sistema de adquisición de datos no bloqueante y replicable.
- Validar experimentalmente el incremento de eficiencia frente a sistemas estáticos en Suchiapa, Chiapas.

## 🏗️ Prototipo Físico y Hardware
A continuación, se presentan las evidencias del diseño, ensamblaje y emplazamiento del hardware utilizado para la experimentación en campo:

<p align="center">
  <img src="images/prototipo_estructura_mecanismo.png" width="17%" alt="Estructura y emplazamiento exterior" />
  <img src="images/prototipo_vista_superior.png" width="32%" alt="Vista superior LDRs y mecanismo" />
  <img src="images/hardware_electronica_sd.png" width="30%" alt="Electrónica de control y Datalogger" />
</p>
<p align="center">
  <em>De izquierda a derecha: 1) Prototipo emplazado para pruebas de campo, 2) Vista superior del panel y distribución de LDRs, 3) Circuito de control con módulo SD para el Datalogger.</em>
</p>


## ⚙️ Lógica del Algoritmo
El sistema utiliza un algoritmo de control que procesa señales analógicas, aplica un filtro de sobremuestreo estadístico para la corriente y gestiona el almacenamiento en SD cada 5 minutos.

```mermaid
graph LR
    A([Inicio]) --> B[Lectura LDRs]
    B --> C[Normalización]
    C --> D{¿Error > Tol?}
    D -- Sí --> E[Ajustar Servo]
    D -- No --> G[Filtrado Corriente]
    E --> G
    G --> H{¿5 min?}
    H -- Sí --> I[Guardar SD]
    H -- No --> B
    I --> B
```

## 📊 Resultados Experimentales
A continuación, se presenta una comparativa del rendimiento obtenida durante las jornadas de prueba en campo:

### Tabla 1. Muestra de Datos del Modelo Dinámico (Intervalo de 1 Hora)

| Temperatura (°C) | Humedad (%) | LDR_Izq | LDR_Der | Ángulo (°) | Corriente (A) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 32.4 | 45.2 | 845 | 842 | 95 | 0.42 |
| 33.1 | 44.1 | 870 | 868 | 102 | 0.48 |
| 34.0 | 42.8 | 910 | 908 | 110 | 0.54 |
| 35.1 | 40.5 | 968 | 965 | 125 | 0.63 |
| 36.0 | 38.5 | 990 | 988 | 135 | 0.68 |

### Tabla 2. Comparativa de Eficiencia: Modelo Estático vs. Dinámico

| Hora del día | Ángulo Dinámico | Corriente Dinámica | Corriente Estática | Ganancia (%) |
| :--- | :---: | :---: | :---: | :---: |
| 09:00 | 45° | 0.38 A | 0.22 A | **+72.7%** |
| 11:00 | 75° | 0.52 A | 0.41 A | **+26.8%** |
| 13:00 | 90° | 0.68 A | 0.65 A | **+4.6%** |
| 15:00 | 115° | 0.58 A | 0.38 A | **+52.6%** |
| 17:00 | 145° | 0.41 A | 0.18 A | **+127.7%** |

## 🛠️ Tecnologías utilizadas
- **Microcontrolador:** Arduino Uno R4 WiFi.
- **Sensores:** LDR (Fotoresistencias), DHT11 (Clima), ACS712 (Corriente).
- **Actuadores:** Servomotor SG90.
- **Almacenamiento:** Módulo Micro SD (SPI).
- **Software:** C++ (Arduino IDE), MATLAB (Análisis de datos).

## ⚙️ Instalación y Dependencias
Para compilar y cargar el código en el microcontrolador, asegúrate de tener instaladas las siguientes librerías en tu **Arduino IDE**:

1. **Servo.h**: (Incluida por defecto en Arduino).
2. **SD.h** y **SPI.h**: (Incluidas por defecto en Arduino).
3. **DHT sensor library** (por Adafruit): Versión 1.4.x o superior.
4. **Adafruit Unified Sensor** (Dependencia necesaria para el DHT).

## 📂 Estructura del repositorio
- `docs/`: Artículos científicos y documentación técnica.
- `hardware/`: Diagramas eléctricos y lista de materiales.
- `src/`: Código fuente (.ino) comentado línea por línea.
- `data/`: Archivos `DATOS.CSV` recolectados en Suchiapa.
- `images/`: Fotografías del prototipo y gráficas de rendimiento.

## 👥 Autores
- **César Emmanuel Llaven Ovilla** – Universidad Politécnica de Chiapas.
- **Samuel Coello García** – Universidad Politécnica de Chiapas.
- **Alejandro Medina Santiago** – INAOE.
- **Christian Roberto Ibáñez Nangüelú** – Universidad Politécnica de Chiapas.

## 📜 Cita y Licencia
Si utilizas este código o los datos para fines académicos, por favor cita de la siguiente manera:
> Ibáñez Nangüelú, C. R. (2026). *Solar Tracker Didáctico: Estación de seguimiento solar y adquisición de datos meteorológicos de bajo costo* [Repositorio de software]. GitHub. https://github.com/cribnez/solar-tracker-didactico/tree/main

Este proyecto se distribuye bajo la licencia **MIT**.
