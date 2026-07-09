#include <Ultrasonic.h>

Ultrasonic ultrasonic(8, 7);

// Motor Direito
#define IN1 3
#define IN2 11

// Motor Esquerdo
#define IN3 6
#define IN4 10

// Sensores de linha
#define LINHA_ESQ 4
#define LINHA_DIR 5

//=========================
// FUNÇÕES DOS MOTORES
//=========================

void atacar() { // Frente máxima velocidade
  analogWrite(IN1, 255);
  analogWrite(IN2, 0);

  analogWrite(IN3, 255);
  analogWrite(IN4, 0);
}

void procurar() { // Giro procurando o adversário
  analogWrite(IN1, 170);
  analogWrite(IN2, 0);

  analogWrite(IN3, 0);
  analogWrite(IN4, 170);
}

void re() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 220);

  analogWrite(IN3, 0);
  analogWrite(IN4, 220);
}

void girarDireita() {
  analogWrite(IN1, 220);
  analogWrite(IN2, 0);

  analogWrite(IN3, 0);
  analogWrite(IN4, 220);
}

void girarEsquerda() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 220);

  analogWrite(IN3, 220);
  analogWrite(IN4, 0);
}

void parar() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);

  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(LINHA_ESQ, INPUT);
  pinMode(LINHA_DIR, INPUT);

  Serial.begin(9600);
}

void loop() {
  bool esq = digitalRead(LINHA_ESQ);
  bool dir = digitalRead(LINHA_DIR);

  // ===== PRIORIDADE: NÃO SAIR DA ARENA =====

  // Os dois sensores detectaram a borda
  if (esq == LOW && dir == LOW) {
    parar();
    delay(20);

    re();
    delay(300);

    girarDireita();
    delay(450);

    return;
  }

  // Apenas sensor esquerdo detectou
  if (esq == LOW) {
    parar();
    delay(20);

    re();
    delay(250);

    girarDireita();
    delay(300);

    return;
  }

  // Apenas sensor direito detectou
  if (dir == LOW) {
    parar();
    delay(20);

    re();
    delay(250);

    girarEsquerda();
    delay(300);

    return;
  }

  // ===== SENSOR ULTRASSÔNICO =====

  int distancia = ultrasonic.read(CM);

  Serial.print("Distancia: ");
  Serial.println(distancia);

  if (distancia <= 25) {
    atacar();
  } else {
    procurar();
  }

  delay(10);
}