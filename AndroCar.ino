#include <Ultrasonic.h>
#include <SoftwareSerial.h>

//Motor Controll pins
#define LEFT_DIR 2   //direction contoll pin of Left Motor
#define LEFT_PWM 3   //Speed Contorr Pin of left motor
#define RIGHT_DIR 4  //direction contoll pin of right motor
#define RIGHT_PWM 5  //Speed Contorr Pin of right motor
// indicators
#define LEFT_IND 2   //left indicator
#define RIGHT_IND 3  //right indicator
//Bluetooth UART pins
#define RX 10  //UART Receive pin connect to TX pin of HC05
#define TX 11  //UART Transmit pin connect to RX pin of HC05
//Ultra sonic
#define TRIG 12  //Triger Pin
#define ECHO 13  //Echo pin


SoftwareSerial BT(RX, TX);
Ultrasonic ultrasonic(TRIG, ECHO);

unsigned distance = 0;
unsigned char speed = 50;
char data = '\0';
char state = 'S';
char status = 0;
void setup() {
  // set the board rate for the bluetooth port
  BT.begin(9600);
  pinMode(LEFT_DIR, OUTPUT);
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(LEFT_IND, OUTPUT);
  pinMode(RIGHT_DIR, OUTPUT);
  pinMode(RIGHT_PWM, OUTPUT);
  pinMode(RIGHT_IND, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (state == 'F') {
    distance = ultrasonic.read();
    if (distance < 30) {
      Stop();
      status = 0;
    } else if (status == 0) {
      Forward();
      status = 1;
    }
  }
  if (BT.available()) {
    if ((data = BT.read()) == 'J') {
      while (!BT.available())
        ;
      speed = BT.read();
      if (speed != 0)
        speed = map(speed, 48, 57, 0, 150);
      if (state == 'F')
        Forward();
      if (state == 'B')
        Reverse();
      if (state == 'L')
        Left();
      if (state == 'R')
        Right();
      Serial.print("Speed =");
      Serial.println(speed);
    } else if (data == 'F') {
      Forward();
      state = 'F';
      status = 1;
    } else if (data == 'B') {
      Reverse();
      state = 'B';
      status = -1;
    } else if (data == 'L') {

      Left();
      state = 'L';
    } else if (data == 'R') {
      Right();
      state = 'R';
    } else if (data == 'S') {               //
      Stop();
      state = 'S';
      status = 0;
    }
    Serial.println(data);
  }
}

void Forward() {
  digitalWrite(LEFT_DIR, 0);
  //digitalWrite(LEFT_PWM, 1);
  analogWrite(LEFT_PWM, speed);
  digitalWrite(RIGHT_DIR, 0);
  //digitalWrite(RIGHT_PWM, 1);
  analogWrite(RIGHT_PWM, speed);
}

void Left() {
  if (status == -1) {
    digitalWrite(LEFT_DIR, 0);
    //digitalWrite(LEFT_PWM,1);
    analogWrite(LEFT_PWM, speed);
    digitalWrite(RIGHT_DIR, 1);
    //digitalWrite(RIGHT_PWM, 0);
    analogWrite(RIGHT_PWM, (255 - speed));
  } else {
    digitalWrite(LEFT_DIR, 1);
    //digitalWrite(LEFT_PWM, 0);
    analogWrite(LEFT_PWM, (255 - speed));
    digitalWrite(RIGHT_DIR, 0);
    //digitalWrite(RIGHT_PWM, 1);
    analogWrite(RIGHT_PWM, speed);
  }
}

void Right() {
  if (status == -1) {
    digitalWrite(LEFT_DIR, 1);
    //digitalWrite(LEFT_PWM,0);
    analogWrite(LEFT_PWM, (255 - speed));
    digitalWrite(RIGHT_DIR, 0);
    //digitalWrite(RIGHT_PWM, 1);
    analogWrite(RIGHT_PWM, speed);
  } else {
    digitalWrite(LEFT_DIR, 0);
    //digitalWrite(LEFT_PWM,1);
    analogWrite(LEFT_PWM, speed);
    digitalWrite(RIGHT_DIR, 1);
    //digitalWrite(RIGHT_PWM, 0);
    analogWrite(RIGHT_PWM, (255 - speed));
  }
}

void Reverse() {
  digitalWrite(LEFT_DIR, 1);
  //digitalWrite(LEFT_PWM, 0);
  analogWrite(LEFT_PWM, (255 - speed));
  digitalWrite(RIGHT_DIR, 1);
  //digitalWrite(RIGHT_PWM, 0);
  analogWrite(RIGHT_PWM, (255 - speed));
}

void Stop() {
  digitalWrite(LEFT_DIR, 0);
  //digitalWrite(LEFT_PWM, 0);
  analogWrite(LEFT_PWM, 0);
  digitalWrite(RIGHT_DIR, 0);
  //digitalWrite(RIGHT_PWM, 0);
  analogWrite(RIGHT_PWM, 0);
}
