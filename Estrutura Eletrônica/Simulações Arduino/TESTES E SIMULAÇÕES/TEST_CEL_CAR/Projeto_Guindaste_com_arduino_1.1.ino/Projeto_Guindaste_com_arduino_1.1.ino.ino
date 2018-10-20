#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <string.h>
#include <stdlib.h>

 
#define VEL_MAX 300 //velocidade máxima do carrinho 320 cm/s
//==========================VARIÁVEIS P/ BLUETOOTH====================
SoftwareSerial serial(10,12); //cria um objeto da classe SoftwareSerial, primeiro parametro é o pino que liga à porta TX e segundo à porta RX 
String content = ""; //variável que receberá os dados do bluetooth
//=============================MOTOR DIREITO==========================
#define ENA 11
#define IN1 9
#define IN2 8
//=============================MOTOR ESQUERDO=========================
#define IN3 7
#define IN4 6
#define ENB 5
//======================VARIÁVEIS P/ FUNÇÕES INTERRUPT================
const byte MOTOR_A = 3;  // Pino interrupt para motor direito
const byte MOTOR_B = 2;  // Pino interrupt para motor esquerdo
unsigned int counter_A = 0; //Inteiro para contagem de pulsos do motor direito
unsigned int counter_B = 0; //Inteiro para contagem de pulsos do motor esquerdo
const float diskslots = 20.00; //número de furos do encoder
const float diametro_roda = 6.70; //diametro da roda em cm
int k = 5;
//========================VARIÁVEIS DE CONTROLE=======================
#define Ki_A 4.2673  //ganho do controlador PI para motor direito
#define Ki_B 4.0707  //ganho do controlador PI para motor esquerdo
#define Kp_A 1.1717   //ganho DC do motor direito
#define Kp_B 1.2283   //ganho DC do motor esquerdo
long lastProcess = 0;  //acumula o tempo de processamento para auxiliar calculo do delta_t
//==============================MOTORES===============================
typedef struct{
  float velocidade = 0;  //set point do motor - velocidade desejada
  float velocidade_medida = 0; //velocidade media medida pelo sensor
  int direcao_1 = "LOW";
  int direcao_2 = "LOW";  //nível lógico das saídas 1 e 2
  int PWM = 0;  //valores de PWM para o motor
  float u = 0;      //ação de controle em cm/s
}motor;

motor motor_A; //motor direito
motor motor_B; //motor esquerdo
//====================================================================
 
 
 
//=================DEFINE SET POINT PARA  CADA MOTOR==================
void SetPoint(float velocidade, float angulo)
{
  int v = 0; //variável auxiliar que armazenará a velocidade máxima em curvas
 
    if(velocidade > 0) //usuario deseja mover carrinho para frente
    {
      motor_A.direcao_1 = HIGH;
      motor_A.direcao_2 = LOW;
      motor_B.direcao_1 = HIGH;
      motor_B.direcao_2 = LOW;
  
      motor_A.velocidade = fabs(velocidade)*VEL_MAX;
      motor_B.velocidade = fabs(velocidade)*VEL_MAX;
     
    }
    else if(velocidade < 0)               //usuario deseja mover carrinho para tras
    {
      motor_A.direcao_1 = LOW;
      motor_A.direcao_2 = HIGH;
      motor_B.direcao_1 = LOW;
      motor_B.direcao_2 = HIGH;   
  
      motor_A.velocidade = fabs(velocidade)*VEL_MAX;
      motor_B.velocidade = fabs(velocidade)*VEL_MAX;
    }
  
    else if(velocidade == 0)               //usuario não acelerou/acionou o movimento de frenagem
    {
      motor_A.direcao_1 = HIGH;
      motor_A.direcao_2 = HIGH;
      motor_B.direcao_1 = HIGH;
      motor_B.direcao_2 = HIGH;   
  
      motor_A.velocidade = fabs(velocidade)*VEL_MAX;
      motor_B.velocidade = fabs(velocidade)*VEL_MAX;
      motor_A.u = 0;
      motor_B.u = 0;
      motor_A.velocidade = 0;
      motor_B.velocidade = 0;
      motor_A.PWM = 0;
      motor_B.PWM = 0;
    } 
    
    if(angulo > 0)    //usuario deseja virar para esquerda
    {
         v = VEL_MAX;
         motor_A.velocidade = v - fabs(angulo)*10*v;
         motor_B.velocidade = v;
    }
    else  if(angulo < 0)         //usuario deseja virar para esquerda
    {
         v = VEL_MAX;
         motor_A.velocidade = v;
         motor_B.velocidade = v - fabs(angulo)*10*v;
    }
 
}
//====================================================================
 
//============================CONTROLADOR=============================
void controlador(motor *m, float delta_t, float Ki, float Kp, int PWM_minimo)
{
    m->PWM = PWM_minimo + m->PWM + Ki*(m->velocidade - m->velocidade_medida)*delta_t/Kp;
    if(m->PWM > 255){ m->PWM = 255;}
    if(m->PWM < 0){ m->PWM = 0;}
}
//====================================================================
 
//=====================MOVIMENTAÇÃO DOS MOTORES=======================
void MovimentaMotorDireito(int PWM, int direcao_1, int direcao_2)
{
        digitalWrite(IN1, direcao_1);
        digitalWrite(IN2, direcao_2);
        analogWrite(ENA,PWM);
}
 
void MovimentaMotorEsquerdo(int PWM, int direcao_1, int direcao_2)
{    
        digitalWrite(IN3, direcao_1);
        digitalWrite(IN4, direcao_2);
        analogWrite(ENB,PWM);
}
//====================================================================
//========================FUNÇÕES INTERRRUPT==========================
void contador_direito() //incrementa contador de interrupções do encoder direito
{
  counter_A++;
}
void contador_esquerdo() //incrementa contador de interrupções do encoder esquerdo
{
  counter_B++;
}

void sensor() //utiliza os interrupts do timer para computar a velocidade dos motores
{
  Timer1.detachInterrupt();                                                         //Para o timer
  float rotation1 = (counter_A / diskslots);                                        // calcula Rotações por segundo para motor direito
  float vA_medida = rotation1 * (3.14 * diametro_roda)*20;                          //calcula velocidade em cm/s para o motor direito
  motor_A.velocidade_medida = (vA_medida + (k-1)*motor_A.velocidade_medida)/k;      //filtra o sinal
  counter_A = 0;                                                                    //reseta o contador

  float rotation2 = (counter_B / diskslots);                                        // calcula Rotações por segundo para motor esquerdo
  float vB_medida = rotation2 * (3.14 * diametro_roda)*20;                          //calcula velocidade em cm/s para o motor esquerdo
  motor_B.velocidade_medida = (vB_medida + (k-1)*motor_B.velocidade_medida)/k;      //filtra o sinal
  counter_B = 0;                                                                    //reseta o contador
  controlador(&motor_A, 0.05, Ki_A, Kp_A, 49);
  controlador(&motor_B, 0.05, Ki_B, Kp_B, 56);
  Timer1.attachInterrupt( sensor );                                                 //habilita o timer
}
//====================================================================
 
void setup() {
  serial.begin(9600);
  Serial.begin(9600);   //para utilizar o monitor serial
 
  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
 
 
  Timer1.initialize(50000); // define como 0.05s o período a serem geradas interrupções - entrada em microsegundos
  attachInterrupt(digitalPinToInterrupt (MOTOR_A), contador_direito, RISING);  //Incrementa o contador direito quando o sensor vai para nível lógico 1
  attachInterrupt(digitalPinToInterrupt (MOTOR_B), contador_esquerdo, RISING);  //Incrementa o contador esquerdo quando o sensor vai para nível lógico 1
  Timer1.attachInterrupt( sensor ); //Atribui uma função para ser chamada a cada interrupção gerada pelo timer
 
}

void loop() {
  
  if (serial.available()) 
  {
    content = serial.readStringUntil('%');
 
    // Convert from String Object to String.
    char buf[20];
    content.toCharArray(buf, sizeof(buf));
    char *p = buf;
    char *str;
    float parametros[3];
    int k =0;
    
    while ((str = strtok_r(p, "|", &p)) != NULL)
    { // delimiter is the semicolo
      parametros[k]=atof(str);
      k++;
    }
    
    if(parametros[2] == 1) //freio foi acionado
    {
        MovimentaMotorDireito(0, HIGH, HIGH);
        MovimentaMotorEsquerdo(0, HIGH, HIGH); 
        motor_A.u = 0;
        motor_B.u = 0;
        motor_A.velocidade = 0;
        motor_B.velocidade = 0;
        motor_A.PWM = 0;
        motor_B.PWM = 0;
    }
    else
    {
        SetPoint(parametros[0], parametros[1]);
        MovimentaMotorDireito(motor_A.PWM, motor_A.direcao_1, motor_A.direcao_2);
        MovimentaMotorEsquerdo(motor_B.PWM, motor_B.direcao_1, motor_B.direcao_2);
    }
  }
}
