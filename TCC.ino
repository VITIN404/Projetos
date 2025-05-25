#include <Servo.h>
#include <SoftwareSerial.h>

// Define conexões do Bluetooth
SoftwareSerial bluetooth(2, 3); // RX, TX

// Define os servos
Servo servoBraço;  // Servo que controla o movimento do braço
Servo servoMão;    // Servo que controla a posição da mão
Servo servoGarra;  // Servo que abre e fecha a garra

// Pinos dos servos
int pinoBraço = 9;
int pinoMão = 10;
int pinoGarra = 11;

// Variáveis para armazenar ângulos dos servos
int anguloBraço = 90;  // Ângulo inicial do braço
int anguloMão = 90;    // Ângulo inicial da mão
int anguloGarra = 90;  // Ângulo inicial da garra

// Variável para comandos Bluetooth
char t;

void setup() {
  // Configuração dos pinos dos motores
  pinMode(5, OUTPUT);   // Motores da esquerda para frente
  pinMode(6, OUTPUT);   // Motores da esquerda para trás
  pinMode(11, OUTPUT);  // Motores da direita para frente
  pinMode(10, OUTPUT);  // Motores da direita para trás
  pinMode(9, OUTPUT);   // LED
  
  // Inicializa comunicação serial para Bluetooth
  bluetooth.begin(9600);
  Serial.begin(9600);

  // Conectando os servos aos pinos
  servoBraço.attach(pinoBraço);
  servoMão.attach(pinoMão);
  servoGarra.attach(pinoGarra);

  // Posiciona os servos na posição inicial
  servoBraço.write(anguloBraço);
  servoMão.write(anguloMão);
  servoGarra.write(anguloGarra);

  Serial.println("Sistema pronto. Aguardando comandos via Bluetooth...");
}

void loop() {
  if (bluetooth.available()) {
    t = bluetooth.read();
    Serial.println(t);
  }

  // Controle simplificado dos motores (4 botões)
  if (t == 'F') {            // Move para frente
    digitalWrite(5, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(10, LOW);
  } 
  else if (t == 'B') {       // Move para trás
    digitalWrite(6, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(11, LOW);
  } 
  else if (t == 'L') {       // Vira à esquerda
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(11, HIGH);
    digitalWrite(10, LOW);
  } 
  else if (t == 'R') {       // Vira à direita
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
  } 
  else if (t == 'S') {       // Para todos os motores
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
  }

  // Controle simplificado da garra (4 botões)
  if (t == 'U') {  // Levanta o braço e fecha a garra ao mesmo tempo
    anguloBraço += 10;
    anguloGarra -= 10;
    if (anguloBraço > 180) anguloBraço = 180;
    if (anguloGarra < 0) anguloGarra = 0;
    servoBraço.write(anguloBraço);
    servoGarra.write(anguloGarra);
  }
  else if (t == 'D') {  // Abaixa o braço e abre a garra ao mesmo tempo
    anguloBraço -= 10;
    anguloGarra += 10;
    if (anguloBraço < 0) anguloBraço = 0;
    if (anguloGarra > 180) anguloGarra = 180;
    servoBraço.write(anguloBraço);
    servoGarra.write(anguloGarra);
  }
  else if (t == 'O') {  // Levanta a mão (abre para pegar algo)
    anguloMão += 10;
    if (anguloMão > 180) anguloMão = 180;
    servoMão.write(anguloMão);
  }
  else if (t == 'C') {  // Abaixa a mão (fecha para pegar algo)
    anguloMão -= 10;
    if (anguloMão < 0) anguloMão = 0;
    servoMão.write(anguloMão);
  }

  // Exibe os ângulos atuais dos servos
  Serial.print("Braço: ");
  Serial.print(anguloBraço);
  Serial.print(" | Mão: ");
  Serial.print(anguloMão);
  Serial.print(" | Garra: ");
  Serial.println(anguloGarra);

  delay(100);
}
