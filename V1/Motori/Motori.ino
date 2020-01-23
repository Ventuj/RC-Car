#include <IRremote.h>

const int rec = 5;
IRrecv irrecv(rec);
decode_results results;

int green = 4;
int red = 3;

int sx1 = 8;
int sx2 = 9;
int dx1 = 10;
int dx2 = 11;

const int trig = 7;
const int echo = 6;

int buzzer = 12;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  
  pinMode(dx1, OUTPUT);
  pinMode(dx2, OUTPUT);
  pinMode(sx1, OUTPUT);
  pinMode(sx2, OUTPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  pinMode(buzzer, OUTPUT);
  
  digitalWrite(red,HIGH);
  ferma();
}

void ferma(){
  digitalWrite(dx1, HIGH);
  digitalWrite(dx2, HIGH);
  digitalWrite(sx1, HIGH);
  digitalWrite(sx2, HIGH);
}

void vai(){
  digitalWrite(dx1, HIGH);
  digitalWrite(dx2, LOW);
  digitalWrite(sx1, LOW);
  digitalWrite(sx2, HIGH);  
}

void retro(){
  digitalWrite(dx1, LOW);
  digitalWrite(dx2, HIGH);
  digitalWrite(sx1, HIGH);
  digitalWrite(sx2, LOW);
}

unsigned long ok = 0xFF02FD;
unsigned long to = 0xFF52AD;
unsigned long ava = 0xFF629D;
unsigned long ind = 0xFFA857;
unsigned long dx = 0xFFC23D;
unsigned long sx = 0xFF22DD;

//dx FFC23D
//sx FF22DD

bool check = false;
bool tono = false;

bool avanti = false;
bool indietro = false;
bool manovra = false;

long duration;
int distance;

void loop() {
  control();
  if(check && !manovra){
    if(distance < 5){
      manovra = false;
      tone(buzzer, 500);
      retro();
      avanti = false;
      delay(100);
      noTone(buzzer);
      delay(200);
      ferma();
    }
  }
  if (irrecv.decode(&results)){
    if(results.value == ok){
      if(check){
        avanti = indietro = check = false;
        digitalWrite(green, LOW);
        digitalWrite(red, HIGH);
        ferma();
      }else{
        check = true;
        digitalWrite(green, HIGH);
        digitalWrite(red, LOW);
      }
    }
    if(check){
      if(results.value == ava){
        if(avanti){
          avanti = false;
          ferma();
        }else{
          avanti = true;
          indietro = false;
          vai();
        }
      }
      if(results.value == ind){
        if(indietro){
          indietro = false;
          ferma();
        }else{
          indietro = true;
          avanti = false;
          retro();
        }
      }
      if(results.value == dx){
        manovra = true;
        if(avanti || indietro){
          if(avanti){
            digitalWrite(sx1, HIGH);
            digitalWrite(sx2, LOW);
            digitalWrite(dx1, HIGH);
            digitalWrite(dx2, LOW);
            delay(200);
            vai();
          }else{
            digitalWrite(sx1, HIGH);
            digitalWrite(sx2, LOW);
            digitalWrite(dx1, HIGH);
            digitalWrite(dx2, LOW);
            delay(200);
            retro(); 
          }
        }else{
          digitalWrite(sx1, HIGH);
          digitalWrite(sx2, LOW);
          digitalWrite(dx1, HIGH);
          digitalWrite(dx2, LOW);
          delay(200);
          ferma();
        }
        manovra = false;
      }
      if(results.value == sx){
        manovra = true;
        if(avanti || indietro){
          if(avanti){
            digitalWrite(sx1, LOW);
            digitalWrite(sx2, HIGH);
            digitalWrite(dx1, LOW);
            digitalWrite(dx2, HIGH);
            delay(200);
            vai();
          }else{
            digitalWrite(sx1, LOW);
            digitalWrite(sx2, HIGH);
            digitalWrite(dx1, LOW);
            digitalWrite(dx2, HIGH);
            delay(200);
            retro(); 
          }
        }else{
          digitalWrite(sx1, LOW);
          digitalWrite(sx2, HIGH);
          digitalWrite(dx1, LOW);
          digitalWrite(dx2, HIGH);
          delay(200);
          ferma();
        }
        manovra = false;
      }
    }
    if(results.value == to){
      if(tono){
        tono = false;
        noTone(buzzer);
      }else{
        tono = true;
        tone(buzzer, 400);
      }
    }
    Serial.println(results.value, HEX);
    irrecv.resume();
  }
}

void control(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration*0.034/2;
  Serial.println(distance);
}
