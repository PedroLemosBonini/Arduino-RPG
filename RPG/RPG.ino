#define botao_01 4
#define botao_02 2
#define ledh 6
#define ledv 5
#define ledamareloh 9
#define ledamarelov 8
#define buzzer 12

bool status = false;
bool status_jogo = false;
bool status02 = true;
bool status03 = true;
bool status04 = true;


int dado_heroi;
int dado_vilao;
int vida;
int vida02;
int vida_heroi;
int vida_vilao;
int total_dano;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));  //para que faça a função random
  pinMode(botao_01, INPUT);
  pinMode(botao_02, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledamareloh, OUTPUT);
  pinMode(ledamarelov, OUTPUT);
  Serial.println("Instrucoes:"); //Instrução do jogo
  Serial.println("Quando o jogo iniciar, aperte o botao do heroi e do vilao alternadamente para rolar os dados, comecando com o do heroi.");
  Serial.println("a diferenca dos dados sera o dano causado naquele que tirou o menor numero");
  Serial.println("Ganha aquele que tornar a vida do oponente igual ou menor que zero");
  Serial.println();
  Serial.println("aperte o botao do lado do led azul (heroi) para iniciar o jogo!");
}

void loop() {  //Faz o jogo iniciar quando o primeiro botao for acionado(herói)
  if (digitalRead(botao_01) == 1 && status_jogo == false || status_jogo == true) {
    status_jogo = true;
    jogo();
  }
}


void configurarVida_heroi() {
  while (Serial.available() > 0) {  // espera até que algo seja digitado
    vida = Serial.parseInt();       //recebe valores inteiro que vc digitar para o herói
    if (vida > 0) {
      vida_heroi = vida;
      status03 = false;
      Serial.println(vida);
    }
  }
}

void configurarVida_vilao() {
  while (Serial.available() > 0) {  // espera até que algo seja digitado
    vida02 = Serial.parseInt();     //recebe valores inteiro que vc digitar para o vilão
    if (vida02 > 0) {
      vida_vilao = vida02;
      status04 = false;
      Serial.println(vida02);
    }
  }
}


void jogo() {
  if (status03 == true) {
    Serial.print("Digite a vida do heroi: ");  
    while (status03 == true) {
      configurarVida_heroi();
    }
  }
  if (status04 == true) {
    Serial.print("Digite a vida do vilao: ");
    while (status04 == true) {
      configurarVida_vilao();
    }
  }
  while (status02 == true) {
    Serial.println("-------------------------------------------------------------");
    Serial.println("aperte o botão do herói para rolar o dado");
    status02 = false;
  }

  // Gera um número aleatório entre 1 a 6 para o herói
  if (digitalRead(botao_01) == 1 && status == false) {
    status = true;
    dado_heroi = random(1, 7); 
    Serial.print("valor do dado do heroi: ");
    Serial.println(dado_heroi);
    for (int i = 1; i <= dado_heroi; i++) {
      tone(buzzer, 523);
      delay(200);
      noTone(buzzer);
      delay(200);
    }
    delay(100);
  }

  //Gera um número aleatório entre 1 a 6 para o vilão
  if (digitalRead(botao_02) == 1 && status == true) {
    status = false;
    dado_vilao = random(1, 7);
    Serial.print("valor do dado do vilao: ");
    Serial.println(dado_vilao);
    for (int i = 1; i <= dado_vilao; i++) {
      tone(buzzer, 523);
      delay(200);
      noTone(buzzer);
      delay(200);
    }
    Serial.println("-------------------------------------------------------------");

    //dano na vida do vilão
    if (dado_vilao < dado_heroi) {
      total_dano = dado_heroi - dado_vilao;
      vida_vilao = vida_vilao - total_dano;
      Serial.print("Dano causado no vilao: ");
      Serial.println(total_dano);
      for (int i = 0; i <= 2; i++) {
        digitalWrite(ledv, 1);
        delay(300);
        digitalWrite(ledv, 0);
        delay(300);
      }
      Serial.print("Vida atual do vilao: ");
      if (vida_vilao < 0) {
        Serial.println("0");
      } else {
        Serial.println(vida_vilao);
      }
      Serial.print("Vida atual do heroi: ");
      Serial.println(vida_heroi);
    }

    //dano na vida do heroi
    else if (dado_heroi < dado_vilao) {
      total_dano = dado_vilao - dado_heroi;
      vida_heroi = vida_heroi - total_dano;
      Serial.print("Dano causado no heroi: ");
      Serial.println(total_dano);
      for (int i = 0; i <= 2; i++) {
        digitalWrite(ledh, 1);
        delay(300);
        digitalWrite(ledh, 0);
        delay(300);
      }
      Serial.print("Vida atual do vilao: ");
      Serial.println(vida_vilao);
      Serial.print("Vida atual do heroi: ");
      if (vida_heroi < 0) {
        Serial.println("0");
      } else {
        Serial.println(vida_heroi);
      }

    } else {
      Serial.println("Os dados sao iguais, ninguem recebe dano!");
    }
    Serial.println("-------------------------------------------------------------");
  }

  //Comunica aos jogadores quem venceu e toca sua respectiva música, além de piscar os leds amarelos para o vencedor
  if (vida_heroi <= 0) {
    Serial.println("A vitoria foi do vilao");
    musica_vilao();
    for (int i = 0; i <= 2; i++) {
      digitalWrite(ledamarelov, 1);
      delay(300);
      digitalWrite(ledamarelov, 0);
      delay(300);
    }
    Serial.println("Aperte o botao ao lado do heroi para reiniciar o jogo!");
    ganhador();
  } 
  else if (vida_vilao <= 0) {
    Serial.println("A vitoria foi do heroi");
    musica_heroi();
    for (int i = 0; i <= 2; i++) {
      digitalWrite(ledamareloh, 1);
      delay(300);
      digitalWrite(ledamareloh, 0);
      delay(300);
    }
    Serial.println("Aperte o botao ao lado do heroi para reiniciar o jogo!");
    ganhador();
  }
}

// redefine os valores para um próximo jogo
void ganhador() {
  status = false;
  status_jogo = false;
  status03 = true;
  status02 = true;
  status04 = true;
}

void musica_heroi() {
  tone(buzzer, 392.00);
  delay(100);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 392.00);
  delay(100);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 392.00);
  delay(100);
  noTone(buzzer);
  delay(100);

  tone(buzzer, 392.00);
  delay(500);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 311.645);
  delay(500);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 349.23);
  delay(500);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 392.00);
  delay(300);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 349.23);
  delay(100);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 392.00);
  delay(1000);
  noTone(buzzer);
  delay(50);
}

void musica_vilao() {
  //dó, sol, mi, lá, si, lá, láb, sib, láb, sol, fá, sol
  delay(1000);
  tone(buzzer, 523);  //dó
  delay(400);
  noTone(buzzer);
  delay(250);
  tone(buzzer, 392);  //sol
  delay(400);
  noTone(buzzer);
  delay(150);
  tone(buzzer, 330);  //mi
  delay(400);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 440);  //lá
  delay(200);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 494);  //si
  delay(200);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 440);  //lá
  delay(200);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 415);  //sol# ou láb
  delay(200);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 466);  //lá# ou sib
  delay(200);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 415);  //sol# ou láb
  delay(200);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 392);  //sol
  delay(100);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 349);  //fá
  delay(100);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 392);  //sol
  delay(700);
  noTone(buzzer);
  delay(50);
}