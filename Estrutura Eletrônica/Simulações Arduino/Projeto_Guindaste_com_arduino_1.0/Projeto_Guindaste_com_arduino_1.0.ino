// ---- INICIO Mapeamento do Hardware 

//ARDUINO
  #define descer 13  //descer
  #define subir 12  //subir
  #define esquerda 11  //esquerda
  #define direita 10  //direita
  
  #define enA 9 //ENA
  #define in1 8 //IN1
  #define in2 7 //IN2
  #define in3 5 //IN3
  #define in4 4 //IN4
  #define enB 3 //ENB

//SENSOR DE VELOCIDADE ENCODER
  #define   vcc      2 //defini as portas para não ficar vago.
  #define   gnd      0
  #define   pino_D0  1
  #define   analog   A5

//DRIVER L298N - PONTE H
//int enA = 9;
//int in1 = 8;
//int in2 = 7;
//
//int in3 = 5;
//int in4 = 4;
//int enB = 3;
// ---- FIM Mapeamento do Hardware 

// --- Variáveis Globais --- 

int           rpm;//,descer,subir,direita,esquerda;
int ponto_inicial = 0;
volatile byte pulsos;
unsigned long timeold;

//Altere o numero abaixo de acordo com o seu disco encoder
unsigned int pulsos_por_volta = 20;


// ========================================================================================================
// --- Interrupção ---
void contador()
{
  //Incrementa contador
  pulsos++;
}


// ========================================================================================================

// --- Configurações Iniciais ---
void setup() {
  // put your setup code here, to run once:
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);//vide tabela : 
  pinMode(in2, OUTPUT);//ponto morto do motor
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(subir,    INPUT);
  pinMode(descer,   INPUT);
  pinMode(direita,  INPUT);
  pinMode(esquerda, INPUT);
  
  pinMode(vcc,    OUTPUT);
  pinMode(gnd,    OUTPUT);
  pinMode(pino_D0, INPUT);
  pinMode(analog,  INPUT);

  digitalWrite(vcc, HIGH);
  digitalWrite(gnd,  LOW);
  
  //Interrupcao 1 - pino digital 1
  //Aciona o contador a cada pulso
  attachInterrupt(1, contador, FALLING);
  pulsos = 0;
  rpm = 0;
  timeold = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
 motor1();
 motor2();
//  
//  digitalWrite(output, HIGH);
//  delay(1000);
//  digitalWrite(output, LOW);
//  delay(1000); 

//Atualiza contador a cada segundo
  if (millis() - timeold >= 1000)
  {
    //Desabilita interrupcao durante o calculo
    detachInterrupt(1);
    rpm = (60 * 1000 / pulsos_por_volta ) / (millis() - timeold) * pulsos;
    timeold = millis();
    pulsos = 0;

    //Mostra o valor de RPM no serial monitor
    Serial.print("RPM = ");
    Serial.println(rpm, DEC);
    //Habilita interrupcao
    attachInterrupt(1, contador, FALLING);
  }

}

//configuração para subir e descer a corda do guindaste
void motor1() {

  if (subir == HIGH and ponto_inicial != 0){ //criar a variavel ponto inicial para ter uma referencia de até quando subir a corda do guindaste
    while(ponto_inicial != 0 ){//or subir = LOW
    digitalWrite(enA, HIGH);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    delay(1000); 
    ponto_inicial--;
    }
  }
  else{
    //nada precisa ser feito? 
    }
  if (descer == HIGH){
    digitalWrite(enA, HIGH);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    delay(1000); 
    ponto_inicial++;
    }
}

//configuração para girar o guindaste
void motor2() {

  if(direita == HIGH ){ //and esquerda = LOW
    digitalWrite(enB, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(1000); 
  }
 
  if (esquerda == HIGH){ //direita = LOW and 
    digitalWrite(enB, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(1000); 
  }

}

