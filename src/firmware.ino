#include <Servo.h>
#include <SPI.h>
#include <SD.h>
#include <DHT.h>

// Definición del pin de datos para el sensor ambiental
#define DHTPIN 3
// Definición del tipo de sensor utilizado
#define DHTTYPE DHT11
// Creación de la instancia para el sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Objeto para la manipulación de archivos en la SD
File myFile;
// Pin de control para el módulo de memoria SD
const int chipSelect = 10;

// Objeto para el control del servomotor
Servo miServo;
// Pin analógico para el sensor de luz izquierdo
const int pinLDR1 = A0;
// Pin analógico para el sensor de luz derecho
const int pinLDR2 = A1;
// Pin analógico para el sensor de corriente Hall
const int pinCorriente = A2; 
// Pin digital con salida PWM para el servo
const int pinServo = 9;

// Variable para el almacenamiento de la lectura izquierda procesada
int valorLDR1 = 0;
// Variable para el almacenamiento de la lectura derecha procesada
int valorLDR2 = 0;
// Variable para rastrear la posición angular del panel
int posicionServo = 90;
// Umbral de diferencia para activar el movimiento (zona muerta)
int tolerancia = 40;
// Almacén del valor calculado de corriente en Amperios
float corrientePanel = 0.0;  
// Variable para el porcentaje de humedad relativa
float h = 0;
// Variable para la temperatura ambiente en grados Celsius
float t = 0;

// Almacén del tiempo de la última ejecución del datalogger
unsigned long tiempoAnterior = 0;
// Intervalo definido para el registro de datos (5 minutos)
const long intervaloGuardado = 300000;

void setup() {
  // Inicialización del puerto serie para monitoreo
  Serial.begin(9600);

  // Vinculación del servomotor al pin correspondiente
  miServo.attach(pinServo);
  // Posicionamiento inicial del panel al cenit
  miServo.write(posicionServo);
  // Activación del sensor de temperatura y humedad
  dht.begin();

  Serial.print("Iniciando SD ...");
  // Intento de conexión con el sistema de archivos de la SD
  if (!SD.begin(chipSelect)) {
    Serial.println("Fallo SD");
    // Bloqueo del sistema en caso de error de hardware
    while (1);
  }
  Serial.println("Exito.");

  // Verificación de existencia del archivo para evitar duplicar encabezados
  if (!SD.exists("DATOS.CSV")) {
    // Apertura del archivo en modo escritura
    myFile = SD.open("DATOS.CSV", FILE_WRITE);
    if (myFile) {
      // Escritura de la fila de encabezados para formato Excel
      myFile.println("Temperatura(C),Humedad(%),LDR_Izq,LDR_Der,Angulo,Corriente(A)");
      // Cierre del archivo para guardar cambios
      myFile.close();
      Serial.println("Archivo nuevo creado con encabezados.");
    }
  }
}

void loop() {
  // Captura de la señal analógica del LDR izquierdo
  int crudoLDR1 = analogRead(pinLDR1);
  // Captura de la señal analógica del LDR derecho
  int crudoLDR2 = analogRead(pinLDR2);

  // Aplicación de límites de seguridad para la lectura izquierda
  crudoLDR1 = constrain(crudoLDR1, 10, 980);
  // Aplicación de límites de seguridad para la lectura derecha
  crudoLDR2 = constrain(crudoLDR2, 57, 1017);

  // Normalización de la lectura izquierda a escala 0-1000
  valorLDR1 = map(crudoLDR1, 10, 980, 0, 1000);
  // Normalización de la lectura derecha a escala 0-1000
  valorLDR2 = map(crudoLDR2, 57, 1017, 0, 1000);

  // Evaluación de la diferencia de luz frente a la tolerancia
  if (abs(valorLDR1 - valorLDR2) > tolerancia) {
    // Decisión de giro basada en la intensidad relativa
    if (valorLDR1 > valorLDR2) {
      posicionServo = posicionServo - 1;
    } else if (valorLDR2 > valorLDR1) {
      posicionServo = posicionServo + 1;
    }
  }

  // Protección para no exceder el límite físico superior del servo
  if (posicionServo > 180) posicionServo = 180;
  // Protección para no exceder el límite físico inferior del servo
  if (posicionServo < 0) posicionServo = 0;
  // Actualización física de la posición del panel
  miServo.write(posicionServo);

  // Acumulador para el proceso de filtrado de ruido
  long sumaLecturas = 0;
  // Bucle de sobremuestreo estadístico (500 ciclos)
  for(int i = 0; i < 500; i++) {
    sumaLecturas = sumaLecturas + analogRead(pinCorriente);
  }
  // Cálculo del promedio de las muestras obtenidas
  float promedioLecturas = sumaLecturas / 500.0;
  // Conversión del valor digital a voltaje real
  float voltajeSensor = promedioLecturas * (5.0 / 1023.0);
  // Ecuación de transferencia del sensor ACS712 para obtener Amperios
  corrientePanel = (voltajeSensor - 2.5) / 0.185;
  
  // Condicional para eliminar lecturas negativas por ruido
  if (corrientePanel < 0.0) {
    corrientePanel = 0.0;
  }
  
  // Captura del tiempo actual de funcionamiento
  unsigned long tiempoActual = millis();

  // Comparación de tiempo para ejecución de tarea programada
  if (tiempoActual - tiempoAnterior >= intervaloGuardado) {
    // Actualización de la marca de tiempo de la última captura
    tiempoAnterior = tiempoActual;

    // Obtención de la humedad desde el sensor DHT
    h = dht.readHumidity();
    // Obtención de la temperatura desde el sensor DHT
    t = dht.readTemperature();

    // Verificación de lectura válida (evitar errores de sensor)
    if (isnan(h) || isnan(t)) { h = 0; t = 0; }

    Serial.println(">>> GUARDANDO EN SD <<<");

    // Apertura del archivo para registro de nueva fila de datos
    myFile = SD.open("DATOS.CSV", FILE_WRITE);

    if (myFile) {
      // Registro de temperatura en el CSV
      myFile.print(t);
      myFile.print(",");
      // Registro de humedad en el CSV
      myFile.print(h);
      myFile.print(",");
      // Registro de valor normalizado LDR izquierdo
      myFile.print(valorLDR1);
      myFile.print(",");
      // Registro de valor normalizado LDR derecho
      myFile.print(valorLDR2);
      myFile.print(",");
      // Registro del ángulo actual del servomotor
      myFile.print(posicionServo);
      myFile.print(",");
      // Registro final de la corriente calculada
      myFile.println(corrientePanel);
     
      // Cierre del archivo y confirmación en memoria física
      myFile.close();
    } else {
      // Reporte de error en caso de fallo en el sistema de archivos
      Serial.println("Error SD");
    }
  }
  // Retraso de cortesía para estabilidad del ciclo principal
  delay(100);
}
