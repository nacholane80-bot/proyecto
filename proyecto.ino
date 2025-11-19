// --- Pines Motor Derecho (M0) ---
#define M0_EN_PIN    6  // Pin D6 (Enable/Velocidad)
#define M0_IN1_PIN   5  // Pin D5 (Input 1)
#define M0_IN2_PIN   7  // Pin D7 (Input 2)

// // --- Pines Motor Izquierdo (M1) ---
#define M1_EN_PIN    3  // Pin D3 (Enable/Velocidad)
#define M1_IN1_PIN   2  // Pin D2 (Input 1)
#define M1_IN2_PIN   4  // Pin D4 (Input 2)

// // --- LED --- // //
#define LED_INTEGRADOe D8;

void setupMovimiento() {
  pinMode(M0_EN_PIN, OUTPUT);
  pinMode(M0_IN1_PIN, OUTPUT);
  pinMode(M0_IN2_PIN, OUTPUT);
  pinMode(M1_EN_PIN, OUTPUT);
  pinMode(M1_IN1_PIN, OUTPUT);
  pinMode(M1_IN2_PIN, OUTPUT);
}

// --- Funciones Básicas de Movimiento ---
void avanzar(int velocidad) {
  digitalWrite(M0_IN1_PIN, HIGH); 
  digitalWrite(M0_IN2_PIN, LOW); 
  analogWrite(M0_EN_PIN, velocidad);
  digitalWrite(M1_IN1_PIN, HIGH); 
  digitalWrite(M1_IN2_PIN, LOW); 
  analogWrite(M1_EN_PIN, velocidad);
}
void retroceder(int velocidad) {
  digitalWrite(M0_IN1_PIN, LOW); 
  digitalWrite(M0_IN2_PIN, HIGH); 
  analogWrite(M0_EN_PIN, velocidad);
  digitalWrite(M1_IN1_PIN, LOW); 
  digitalWrite(M1_IN2_PIN, HIGH); 
  analogWrite(M1_EN_PIN, velocidad);
}
void girarDerecha(int velocidad) {
  digitalWrite(M1_IN1_PIN, HIGH); 
  digitalWrite(M1_IN2_PIN, LOW); 
  analogWrite(M1_EN_PIN, velocidad);
  digitalWrite(M0_IN1_PIN, LOW); 
  digitalWrite(M0_IN2_PIN, HIGH); 
  analogWrite(M0_EN_PIN, velocidad);
}
void girarIzquierda(int velocidad) {
  digitalWrite(M1_IN1_PIN, LOW); 
  digitalWrite(M1_IN2_PIN, HIGH); 
  analogWrite(M1_EN_PIN, velocidad);
  digitalWrite(M0_IN1_PIN, HIGH); 
  digitalWrite(M0_IN2_PIN, LOW); 
  analogWrite(M0_EN_PIN, velocidad);
}
void detener() {
  analogWrite(M0_EN_PIN, 0);
  analogWrite(M1_EN_PIN, 0);
}
#define LDR_IZQ_PIN A4
#define LDR_DER_PIN A2

// --- Variables de Calibración ---
// Umbral de luz: qué valor consideramos "suficiente luz" para movernos.
// ¡Calibra este valor! (Usa el sketch del Proyecto 7 para ver valores)
int UMBRAL_LUZ = 400; 

// Tolerancia: diferencia mínima de luz para decidir girar.
// Evita que el robot "vibre" cuando la luz está al frente.
int TOLERANCIA = 50; 

void setup() {
  setupMovimiento();
  Serial.begin(9600); // Opcional, para calibrar
}

void loop() {
  // Leemos los valores de los LDR.
  // IMPORTANTE: Dependiendo de cómo esté hecha la placa del LDR,
  // más luz puede ser un valor MÁS ALTO o MÁS BAJO.
  // Asumiremos que MÁS LUZ = VALOR MÁS ALTO.
  // Si tu robot huye de la luz, invierte la lógica.
  int luzIzq = analogRead(LDR_IZQ_PIN);
  int luzDer = analogRead(LDR_DER_PIN);
  // Opcional: imprimir valores para calibrar
  Serial.print("Izq: "); Serial.print(luzIzq);
  Serial.print(" | Der: "); Serial.println(luzDer);                   

  // --- Lógica de decisión ---

  if (luzIzq < (luzDer + TOLERANCIA)) {
    // 1. Hay más luz a la izquierda
    girarIzquierda(180);
    digitalWrite(LED_INTEGRADO, HIGH); // Encender LED
  } 
  else if (luzDer < (luzIzq + TOLERANCIA)) {
    // 2. Hay más luz a la derecha
    girarDerecha(180);
    digitalWrite(LED_INTEGRADO, HIGH); // Encender LED
  } 
  else {
    // 3. La luz está al frente (o los valores son parecidos)
    // Solo avanzamos si hay suficiente luz
    if (luzIzq > UMBRAL_LUZ && luzDer > UMBRAL_LUZ) {
      avanzar(160);
      digitalWrite(LED_INTEGRADO, HIGH); // Encender LED
    } else {
      // No hay suficiente luz, nos detenemos
      detener();
      digitalWrite(LED_INTEGRADO, HIGH); // Encender LED
    }
  }
}





