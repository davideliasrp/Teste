/************************************************
Editor   : MYBOTIC www.mybotic.com.my
Date     : 29 Aug 2014
Project  : 2 Channels Motor Driver Module
*************************************************/

//define motor 1 related pins
#define IN1 9
#define IN2 8
#define ENA 10

//define motor 2 related pins
#define IN3 7
#define IN4 6
#define ENB 5

//motor 1
int PWM_Value_1 = 0; //PWM value for motor 1 
int chave_1 = 2; //botão  para o motor 1 
byte on_off_1 = 0; //motor ligado 1 , motor desligado 0
//motor 2
int PWM_Value_2 = 0; //PWM value for motor 1 
int chave_2 = 3; //botão  para o motor 2 
byte on_off_2 = 0; //motor ligado 1 , motor desligado 0

//define input pin to test
int esquerda = 13;

void liga_motor_1();
void liga_motor_2();

void setup()
{ 
  //set output for motor 1 related pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  
  //set output for motor 2 related pins
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  //set input for test pins
  pinMode(chave_1, INPUT);
  pinMode(chave_2, INPUT);

  pinMode(esquerda, INPUT);

//    //set motor 1 run in clockwise
//    digitalWrite(IN1, HIGH);
//    digitalWrite(IN2, LOW);
// 
//    //set motor 2 run in anticlockwise
//    digitalWrite(IN3, HIGH);
//    digitalWrite(IN4, LOW);
  //BOTÃO:
  digitalWrite(chave_1, HIGH);
  digitalWrite(chave_2, HIGH);
  
  digitalWrite(esquerda, HIGH);
  
  //INTERRUPÇÃO pino 2 para ativiar o motor 1 por um tempo determinado
  attachInterrupt(0,liga_motor_1,LOW);
  //INTERRUPÇÃO pino 3 para ativiar o motor 1 por um tempo determinado
  attachInterrupt(1,liga_motor_2,LOW);
 
}

void loop()
{
 // int PWM_Value = 0; //PWM value for motor 1 and 2
  
  //verificar se o botão foi pressionado
  if (on_off_1 == 1){
     //set motor 1 run in clockwise
     if (esquerda == LOW){
           //set motor 1 run in clockwise
           digitalWrite(IN1, LOW);
           digitalWrite(IN2, HIGH);
     }
     
     //digitalWrite(IN1, HIGH);
     //digitalWrite(IN2, LOW);
     for(PWM_Value_1 = 0; PWM_Value_1<=255; PWM_Value_1+=5)
        {
          analogWrite(ENA, PWM_Value_1); //set PWM to Motor 1
          //analogWrite(ENB, PWM_Value); //set PWM to Motor 2
          delay(100);
        }
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      on_off_1 = 0;
      PWM_Value_1 = 0;
  }
  if (on_off_2 == 1){
     //set motor 2 run in anticlockwise
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
      for(PWM_Value_2 = 0; PWM_Value_2<=255; PWM_Value_2+=5)
        {
          //analogWrite(ENA, PWM_Value); //set PWM to Motor 1
          analogWrite(ENB, PWM_Value_2); //set PWM to Motor 2
          delay(100);
        }
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      on_off_2 = 0;
      PWM_Value_2 = 0;
      }


  
//  if(PINO13 == HIGH){
//        //Motor 1 and Motor 2 run with gradually increasing speed until Max
//        for(PWM_Value = 0; PWM_Value<=255; PWM_Value+=5)
//        {
//          analogWrite(ENA, PWM_Value); //set PWM to Motor 1
//          analogWrite(ENB, PWM_Value); //set PWM to Motor 2
//          delay(100);
//        }
//  
//      
//        //Motor 1 and Motor 2 run with gradually descreasing speed until Stop
//        for(PWM_Value = 255; PWM_Value>=0; PWM_Value-=5)
//        {
//         // analogWrite(ENA, PWM_Value); //set PWM to Motor 1
//          analogWrite(ENB, PWM_Value); //set PWM to Motor 2
//          delay(100);
//        }
//   }
}

//ciando função para a interrupção.
void liga_motor_1(){
   on_off_1  = 1;  
}
void liga_motor_2(){
   on_off_2  = 1;  
}
  //To do the motors work
//  if (PINO13 == HIGH){
//    //set motor 1 run in clockwise
//    digitalWrite(IN1, LOW);
//    digitalWrite(IN2, HIGH);
//  }
//  else{  
//    //set motor 2 run in anticlockwise
//    digitalWrite(IN3, LOW);
//    digitalWrite(IN4, HIGH);
//  }





// CODIGO DA ISABELAA


//#include <Servo.h>
//
//#define ANGULO_INICIAL_MOTOR 90
//
//// --- Mapeamento dos Joysticks ---
//#define botao 2
//
//// --- Mapeamento dos Servos ---
//Servo motorBase;
//
//void setup() {
//  mapearPinosDosMotores();
//  inicializarAngulosDosMotores();
//  mapearJoysticksComoInput();
//}
//
//void loop() {
//  moverBase();
//  delay(100);
//}
//
//void mapearPinosDosMotores() {
//  motorBase.attach(5);
//}
//
//void inicializarAngulosDosMotores() {
//  motorBase.write(ANGULO_INICIAL_MOTOR);
//}
//
//
//void mapearJoysticksComoInput() {
//  pinMode(botao, INPUT);
//}
//
//void moverBase() {
//  
//  int posX = digitalRead(botao);
//  if(posX == HIGH) {
//    posX = map(posX, 0, 1023, 0, 180);
//    motorBase.write(posX);
//  }
//  else {
//    motorBase.detach();
//  }
//}

