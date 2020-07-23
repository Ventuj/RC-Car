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


//speed
int clockPin = 19; //pin 11 19
int latchPin = 18; //pin 12 18
int dataPin = 17; //pin 14 17


//fari 
int clockPin1 = 14; //pin 11 16 
int latchPin1 = 15; //pin 12 15
int dataPin1 = 16; //pin 14 14 

int pot = A1;

int perc = 80;
//int Speed = (perc * 255) / 100;

int Speed;
int distanza;

int green = 8;
int red = 9;

byte test[] = {
  B00000000,
  B10000000,
  B11000000,
  B11100000,
  B11110000,
  B11111000,
  B11111100,
  B11111110,
  B11111111
};

byte fari[] = {
  B10000001,
  B11000011,
  B11100111,
};

byte fari1[] = {
  B11100111,
  B11000011,
  B10000001,
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
  
  pinMode(dataPin1, OUTPUT);
  pinMode(latchPin1, OUTPUT);
  pinMode(clockPin1, OUTPUT);
  pinMode(pot, INPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  
  digitalWrite(red, HIGH);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Connssione con RC Car avvenuta!");
  Serial.println();

  turnoff();
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
  int val = 0;
  if(faro){
    val = 255;
  }
  //posteriori
  digitalWrite(latchPin1, LOW);
  shiftOut(dataPin1, clockPin1, MSBFIRST, val);
  shiftOut(dataPin1, clockPin1, MSBFIRST, 255);
  digitalWrite(latchPin1, HIGH); 
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
  //posteriori
  digitalWrite(latchPin1, LOW);
  shiftOut(dataPin1, clockPin1, MSBFIRST, val);
  shiftOut(dataPin1, clockPin1, MSBFIRST, 0);
  digitalWrite(latchPin1, HIGH); 
}

void avanti(){
  analogWrite(E1, Speed);
  analogWrite(E2, Speed);
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  digitalWrite(M3, LOW);
  digitalWrite(M4, HIGH);
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
  distanza = map(vpot, 0, 1023, 4, 15);
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
    if(dist[0] < distanza){
      av = false;
      indietro();
      Serial.println("Ho trovato un ostacolo!");
      delay(100);
      ferma();
    }
    if(dist[1] < distanza){
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
        digitalWrite(red, HIGH);
        digitalWrite(green, LOW);
        ferma();
      }else{
        check = true;
        digitalWrite(red, LOW);
        digitalWrite(green, HIGH);
        Serial.println("Macchina avviata");
        Serial.println("Powered By Ventuj-Industries");
      }
    }
    if(ch == 'f'){
      if(faro){
        faro = false;
        for(int i = 0; i < sizeof(fari1); i++){
          digitalWrite(latchPin1, LOW);        
          shiftOut(dataPin1, clockPin1, MSBFIRST, fari1[i]);
          shiftOut(dataPin1, clockPin1, MSBFIRST, 0);
          digitalWrite(latchPin1, HIGH); 
          delay(70);
        }       
        digitalWrite(latchPin1, LOW);      
        shiftOut(dataPin1, clockPin1, MSBFIRST, 0);  
        shiftOut(dataPin1, clockPin1, MSBFIRST, 0);
        digitalWrite(latchPin1, HIGH); 
      }else{
        faro = true;
        for(int i = 0; i < sizeof(fari); i++){
          digitalWrite(latchPin1, LOW);
          shiftOut(dataPin1, clockPin1, MSBFIRST, fari[i]);
          shiftOut(dataPin1, clockPin1, MSBFIRST, 0);
          digitalWrite(latchPin1, HIGH); 
          delay(70);
        }       
        digitalWrite(latchPin1, LOW);      
        shiftOut(dataPin1, clockPin1, MSBFIRST, 255);
        shiftOut(dataPin1, clockPin1, MSBFIRST, 0);
        digitalWrite(latchPin1, HIGH); 
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
          Serial.println("Indietro - OFF");
          ind = false;
          ferma();
        }else{
          Serial.println("Indietro - ON");
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

void turnoff(){
  digitalWrite(latchPin1, LOW);      
  shiftOut(dataPin1, clockPin1, MSBFIRST, 0);
  shiftOut(dataPin1, clockPin1, MSBFIRST, 0);
  digitalWrite(latchPin1, HIGH); 
}
