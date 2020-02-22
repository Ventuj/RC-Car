int led = 8;
//driver
int E1 = 2;
int M1 = 3;
int M2 = 4;
int M3 = 5;
int M4 = 6;
int E2 = 7;
//sensori
int T1 = 22;
int C1 = 23;
int T2 = 24;
int C2 = 25;

int clockPin = 16; //pin 11
int latchPin = 15; //pin 12
int dataPin = 14; //pin 14

int pot = A1;

int perc = 80;
//int Speed = (perc * 255) / 100;

int Speed;

byte test[] = {
  B00000000,
  B00000001,
  B00000011,
  B00000111,
  B00001111,
  B00011111,
  B00111111,
  B01111111,
  B11111111
};

void setup(){
  Serial.begin(9600);
  //EEBlue.begin(9600);
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);
  
  pinMode(T1, OUTPUT);
  pinMode(C1, INPUT);
  pinMode(T2, OUTPUT);
  pinMode(C1, INPUT);
  pinMode(led, OUTPUT);
  //analogWrite(led, 20);
    
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(pot, INPUT);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Connssione con RC Car avvenuta!");
  Serial.println();
}

bool check = false;
bool inserimento = false;

bool av = false;
bool ind = false;
bool manovra = false;

bool faro = false;

int dist[4] = {0,0,0,0};

//movimenti
void ferma(){
  //analogWrite(led, 250);
  for(int i = Speed; i > 20; i--){
    analogWrite(E1, i);
    analogWrite(E2, i);
    delay(5);
  }
  //analogWrite(led, 20);
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  digitalWrite(M3, LOW);
  digitalWrite(M4, LOW);
}

void avanti(){
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  digitalWrite(M3, LOW);
  digitalWrite(M4, HIGH);
  analogWrite(E1, Speed);
  analogWrite(E2, Speed);
}

void indietro(){
  analogWrite(E1, Speed);
  analogWrite(E2, Speed);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  digitalWrite(M3, HIGH);
  digitalWrite(M4, LOW);
}

void dx(){
  analogWrite(E1, Speed);
  analogWrite(E2, Speed);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  digitalWrite(M3, LOW);
  digitalWrite(M4, HIGH);
}

void sx(){
  analogWrite(E1, Speed);
  analogWrite(E2, Speed);
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  digitalWrite(M3, HIGH);
  digitalWrite(M4, LOW); 
}

int distance(int t, int e){
  long duration = 0;
  int distance = 0;
  digitalWrite(t, LOW);
  delayMicroseconds(2);
  digitalWrite(t, HIGH);
  delayMicroseconds(10);
  digitalWrite(t, LOW);
  duration = pulseIn(e, HIGH);
  distance = duration*0.034/2;
  //Serial.println(distance);
  return distance;
}

void velocita(){
  int vpot = analogRead(pot);
  int var = map(vpot, 0,1023, 0, 9);
  Speed = map(vpot, 0,1023, 30, 256);
  digitalWrite(latchPin, LOW);           
  shiftOut(dataPin, clockPin, MSBFIRST, test[var]);
  digitalWrite(latchPin, HIGH);   
  if(av){
    avanti();       
  }else{
    if(ind){
      indietro();
    }
  }
}

void loop() {
  velocita();
  if(check){
    if(dist[0] < 8){
      av = false;
      indietro();
      Serial.println("Ho trovato un ostacolo!");
      delay(100);
      ferma();
    }
    if(dist[1] < 8){
      av = false;
      indietro();
      Serial.println("Ho trovato un ostacolo!");
      delay(100);
      ferma();
    }
  }
  if (Serial.available()){
    char ch = Serial.read();
    if(ch == '0'){
      if(check){
        av = ind = check = false;
        Serial.println("Macchina stoppata");
        ferma();
      }else{
        check = true;
        Serial.println("Macchina avviata");
        Serial.println("Powered By Ventuj-Industries");
      }
    }
    if(ch == 'f'){
      if(faro){
        faro = false;
        digitalWrite(led, LOW);
      }else{
        faro = true;
        digitalWrite(led, HIGH);
      }
    }
    if(check){
      if(ch == '1'){
        if(av){
          av = false;
          Serial.println("Avanti - OFF");
          ferma();
        }else{
          Serial.println("Avanti - ON");
          ind = false;
          av = true;
          avanti();
        }
      }
      if(ch == '2'){
        if(ind){
          Serial.println("Idietro - OFF");
          ind = false;
          ferma();
        }else{
          Serial.println("Idietro - ON");
          av = false;
          ind = true;
          indietro();
        }
      }
      if(ch == '3'){
        manovra = true;
        Serial.println("Destra");
        if(av || ind){
          if(av){
            dx();
            delay(200);
            avanti();
          }else{
            dx();
            delay(200);
            indietro(); 
          }
        }else{
          dx();
          delay(200);
          ferma();
        }
        manovra = false;
      }
      if(ch == '4'){
        manovra = true;
        Serial.println("Sinistra");
        if(av || ind){
          if(av){
            sx();
            delay(200);
            avanti();
          }else{
            sx();
            delay(200);
            indietro(); 
          }
        }else{
          sx();
          delay(200);
          ferma();
        }
        manovra = false;
      }
    }
  }
  dist[0] = distance(T1, C1);
  dist[1] = distance(T2, C2);
}
