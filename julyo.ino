#include <Ultrasonic.h>

// =====================================================
// ULTRASSÔNICO
// =====================================================
#define TRIG_PIN 8
#define ECHO_PIN 7

Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);

// =====================================================
// MOTORES
// Motor Direito
// =====================================================
#define IN1 3
#define IN2 11

// Motor Esquerdo
#define IN3 6
#define IN4 10

// =====================================================
// SENSORES DE LINHA
// =====================================================
#define LINHA_ESQ 4
#define LINHA_DIR 2

// =====================================================
// CONFIGURAÇÕES
// =====================================================

// Velocidades
#define VELOCIDADE_ATAQUE 255
#define VELOCIDADE_BUSCA 180
#define VELOCIDADE_RE 220
#define VELOCIDADE_GIRO 220

// Distância para atacar
#define DISTANCIA_ATAQUE 50

// Tempos de fuga
#define TEMPO_RE_DOIS 300
#define TEMPO_RE_UM   230

#define TEMPO_GIRO_DOIS 420
#define TEMPO_GIRO_UM   300

// =====================================================
// CONTROLE DOS MOTORES
// =====================================================

void atacar() {
  // Ambos os motores para frente
  analogWrite(IN1, VELOCIDADE_ATAQUE);
  analogWrite(IN2, 0);

  analogWrite(IN3, VELOCIDADE_ATAQUE);
  analogWrite(IN4, 0);
}

void procurar() {
  // Giro no próprio eixo
  analogWrite(IN1, VELOCIDADE_BUSCA);
  analogWrite(IN2, 0);

  analogWrite(IN3, 0);
  analogWrite(IN4, VELOCIDADE_BUSCA);
}

void re() {
  // Ambos os motores para trás
  analogWrite(IN1, 0);
  analogWrite(IN2, VELOCIDADE_RE);

  analogWrite(IN3, 0);
  analogWrite(IN4, VELOCIDADE_RE);
}

void girarDireita() {
  // Motor direito para trás
  // Motor esquerdo para frente
  analogWrite(IN1, VELOCIDADE_GIRO);
  analogWrite(IN2, 0);

  analogWrite(IN3, 0);
  analogWrite(IN4, VELOCIDADE_GIRO);
}

void girarEsquerda() {
  // Motor direito para frente
  // Motor esquerdo para trás
  analogWrite(IN1, 0);
  analogWrite(IN2, VELOCIDADE_GIRO);

  analogWrite(IN3, VELOCIDADE_GIRO);
  analogWrite(IN4, 0);
}

void parar() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);

  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

// =====================================================
// FUGA DA BORDA
// =====================================================

void fugirDoisSensores() {
  parar();
  delay(20);

  re();
  delay(TEMPO_RE_DOIS);

  girarDireita();
  delay(TEMPO_GIRO_DOIS);

  parar();
}

void fugirEsquerda() {
  parar();
  delay(20);

  re();
  delay(TEMPO_RE_UM);

  girarDireita();
  delay(TEMPO_GIRO_UM);

  parar();
}

void fugirDireita() {
  parar();
  delay(20);

  re();
  delay(TEMPO_RE_UM);

  girarEsquerda();
  delay(TEMPO_GIRO_UM);

  parar();
}

int lerDistanciaFiltrada() {

  int leituras[5];

  for (int i = 0; i < 5; i++) {
    leituras[i] = ultrasonic.read(CM);
    delay(10);
  }

  // Ordena as 5 leituras
  for (int i = 0; i < 4; i++) {
    for (int j = i + 1; j < 5; j++) {

      if (leituras[j] < leituras[i]) {

        int temp = leituras[i];
        leituras[i] = leituras[j];
        leituras[j] = temp;
      }
    }
  }

  // Retorna a mediana
  return leituras[2];
}


// =====================================================
// SETUP
// =====================================================

void setup() {

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(LINHA_ESQ, INPUT);
  pinMode(LINHA_DIR, INPUT);

  Serial.begin(9600);

  parar();
}

// =====================================================
// LOOP PRINCIPAL
// =====================================================

void loop() {

  bool esq = digitalRead(LINHA_ESQ);
  bool dir = digitalRead(LINHA_DIR);

  int distancia = lerDistanciaFiltrada();

  Serial.print("ESQ: ");
  Serial.print(esq);

  Serial.print(" | DIR: ");
  Serial.print(dir);

  Serial.print(" | DIST: ");
  Serial.print(distancia);
  Serial.println(" cm");


  // =================================================
  // FUGA DA BORDA
  // Branco = 0
  // Preto  = 1
  // =================================================

  if (esq == 0 && dir == 0) {

    fugirDoisSensores();

  } 
  else if (esq == 0) {

    fugirEsquerda();

  } 
  else if (dir == 0) {

    fugirDireita();

  }

  // =================================================
  // COMPORTAMENTO NORMAL
  // =================================================
  else {

    if (distancia > 0 && distancia <= DISTANCIA_ATAQUE) {
      atacar();
    } 
    else {
      // procurar();
      return;
    }

  }

  delay(20);
}



