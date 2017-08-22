#include <IRremote.h>

int musicMode;
int rawVolume;
int volume;
int Rvol;
int Gvol;
int Bvol;
int vol;
int selected2;
int select2trigger;
int MusicLedTimer;

int wakeUpTrigger = 20;
int wakeUpOn;
int wakeUpMode;
int wakeUpPin = A0;
int wakeUpStatus;
int wakeUpRedLEDTimer;
int wakeUpGreenLEDTimer;
int wakeUpWhiteLEDTimer;

int selected;
int R = 5;
int Rval;
int G = 6;
int Gval;
int B = 9;
int Bval;
int W = 10;
int Wval;

int ledTimer;

int commandDelay = 150;

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {

  //setPwmFrequency(5, 8);
  //setPwmFrequency(6, 8);
  //setPwmFrequency(9, 8);
  //setPwmFrequency(10, 256);

  musicMode = 0;

  ledTimer = 0;

  wakeUpOn = 1;
  wakeUpStatus = 0;
  wakeUpWhiteLEDTimer = 0;

  selected = W;

  Rval = 0;
  Gval = 0;
  Bval = 0;
  Wval = 0;

  pinMode(wakeUpPin, INPUT);
  
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(W, OUTPUT);

  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void fastLamp() {
  wakeUpOn = 1;
  while (wakeUpOn == 1) {
    while (wakeUpRedLEDTimer < 200) {
      wakeUpRedLEDTimer++;
      analogWrite(R, wakeUpRedLEDTimer);
      delay(2);
    }
    while (wakeUpGreenLEDTimer < 35) {
      wakeUpGreenLEDTimer++;
      analogWrite(G, wakeUpGreenLEDTimer);
      delay(4);
    }
    while (wakeUpWhiteLEDTimer < 255) {
      wakeUpWhiteLEDTimer++;
      analogWrite(W, wakeUpWhiteLEDTimer);
      delay(3);
    }

    if (results.value == 0x1A545678) {
      wakeUpOn = 0;
      while (wakeUpWhiteLEDTimer > 0) {
        wakeUpWhiteLEDTimer--;
        analogWrite(W, wakeUpWhiteLEDTimer);
        delay(3);
      }
      while (wakeUpGreenLEDTimer > 0) {
        wakeUpGreenLEDTimer--;
        analogWrite(G, wakeUpGreenLEDTimer);
        delay(4);
      }
      while (wakeUpRedLEDTimer > 0) {
        wakeUpRedLEDTimer--;
        analogWrite(R, wakeUpRedLEDTimer);
        delay(2);
      }
      results.value = 0xFFFFFFFF;
    }
    
    if (irrecv.decode(&results)) {
      Serial.println(results.value, HEX);
      irrecv.resume(); // Receive the next value
    }
  
  }
}

void Music1() {
  digitalWrite(13, HIGH);
  while (musicMode == 1) {

    if (irrecv.decode(&results)) {
      Serial.println(results.value, HEX);
      irrecv.resume(); // Receive the next value
    }

    if (results.value == 0x1B245678) { ///WhiteDown
      Wval = Wval - 10;
      analogWrite(W, constrain(Wval, 0, 255));
      delay(commandDelay);
      results.value = 0xFFFFFFFF;
    }

    if (results.value == 0x1B345678) { ///WhiteUp
      Wval = Wval + 10;
      analogWrite(W, constrain(Wval, 0, 255));
      delay(commandDelay);
      results.value = 0xFFFFFFFF;
    }

    if (results.value == 0x1A245678) { ///musicMode off
      musicMode = 0;
      analogWrite(R, Rval);
      analogWrite(G, Gval);
      analogWrite(W, Wval);
      digitalWrite(13, LOW);
      results.value = 0xFFFFFFFF;
      delay(500);
    }
    
    rawVolume = analogRead(wakeUpPin);
    volume = round(rawVolume/4.5); ///Volume is from 0 to 100. Need correction multiplier.
    if ((volume > 1) && (volume < 60)){ ////////////////////RED
      Rvol = round(volume * 0.6 * 25.6);
      analogWrite(R, constrain(Rvol, 10, 255));
      delay(round(Rvol/8));
      delay(4);
    }
    else {
      analogWrite(R, 9);
    }
    rawVolume = analogRead(wakeUpPin);
    volume = rawVolume/5;
    if ((volume > 20) && (volume < 80)) { ////////////////////GREEN
      Gvol = round((volume - 20) * 0.6 * 25.6);
      analogWrite(G, constrain(Gvol, 10, 175));
      delay(round(Gvol/6));
      delay(4);
    }
    else {
      analogWrite(G, 9);
    }
    rawVolume = analogRead(wakeUpPin);
    volume = rawVolume/4;
    if (volume > 40) { ////////////////////BLUE
      Bvol = round((volume - 40) * 0.6 * 25.6);
      analogWrite(B, constrain(Bvol, 10, 155));
      delay(round(Bvol/3));
      delay(4);
    }
    else {
      analogWrite(B, 9);
    }
  }
}

void Music2() {
  digitalWrite(13, HIGH);
  while (musicMode == 2) {

    if (results.value == 0x1A345678) { ///musicMode off
      musicMode = 0;
      analogWrite(R, Rval);
      analogWrite(G, Gval);
      analogWrite(W, Wval);
      digitalWrite(13, LOW);
      results.value = 0xFFFFFFFF;
      delay(500);
    }

    if (irrecv.decode(&results)) {
      Serial.println(results.value, HEX);
      irrecv.resume(); // Receive the next value
    }

    if (results.value == 0x1B245678) { ///WhiteDown
      if (selected == W) {
        Wval = Wval - 10;
        analogWrite(W, constrain(Wval, 0, 255));
        delay(commandDelay);
        results.value = 0xFFFFFFFF;
      }
    }

    if (results.value == 0x1B345678) { ///WhiteUp
      if (selected == W) {
        Wval = Wval + 10;
        analogWrite(W, constrain(Wval, 0, 255));
        delay(commandDelay);
        results.value = 0xFFFFFFFF;
      }
    }

    rawVolume = analogRead(wakeUpPin);
    volume = round(rawVolume/5); ///Volume is from 0 to 100. Need correction multiplier.
    vol = round(volume * 2.56);
    
    if (selected == R) {
      analogWrite(R, constrain(vol, 10, 255));
    }
    if (selected == G) {
      analogWrite(G, constrain(vol, 10, 255));
    }
    if (selected == B) {
      analogWrite(B, constrain(vol, 10, 255));
    }

    if (selected2 == R) {
      analogWrite(R, constrain(vol, 10, 255));
      delay(round(volume));
    }
    if (selected2 == G) {
      analogWrite(G, constrain(vol, 10, 255));
      delay(round(volume));
    }
    if (selected2 == B) {
      analogWrite(B, constrain(vol, 10, 255));
      delay(round(volume));
    }
    delay(2);

    ////////SELECT FIRST COLOR////////
  
    if (results.value == 0x1B545678) {
      selected = R;
      delay(commandDelay);
      results.value = 0xFFFFFFFF;
      analogWrite(G, 0);
      analogWrite(B, 0);
    }
    if (results.value == 0x1B645678) {
      selected = G;
      delay(commandDelay);
      results.value = 0xFFFFFFFF;
      analogWrite(R, 0);
      analogWrite(B, 0);
    }
    if (results.value == 0x1B745678) {
      selected = B;
      delay(commandDelay);
      results.value = 0xFFFFFFFF;
      analogWrite(R, 0);
      analogWrite(G, 0);
    }
    if (results.value == 0x1B845678) {
      selected = W;
      delay(commandDelay);
      results.value = 0xFFFFFFFF;
    }

    ////////SELECT SECOND COLOR////////

    if (results.value == 0x1A445678) {
      digitalWrite(13, LOW);
      select2trigger = 1;
      MusicLedTimer = 0;
      while (select2trigger == 1) {

        if (irrecv.decode(&results)) {
          Serial.println(results.value, HEX);
          irrecv.resume(); // Receive the next value
        }
      
        if (results.value == 0x1B545678) {
          selected2 = R;
          delay(commandDelay);
          results.value = 0xFFFFFFFF;
          analogWrite(G, 0);
          analogWrite(B, 0);
          select2trigger = 0;
          delay(500);
        }
        if (results.value == 0x1B645678) {
          selected2 = G;
          delay(commandDelay);
          results.value = 0xFFFFFFFF;
          analogWrite(R, 0);
          analogWrite(B, 0);
          select2trigger = 0;
          delay(500);
        }
        if (results.value == 0x1B745678) {
          selected2 = B;
          delay(commandDelay);
          results.value = 0xFFFFFFFF;
          analogWrite(R, 0);
          analogWrite(G, 0);
          select2trigger = 0;
          delay(500);
        }
        
        if (MusicLedTimer == 400) {
          digitalWrite(13, HIGH);
        }
        if (MusicLedTimer == 800) {
          digitalWrite(13, LOW);
          MusicLedTimer = 0;
        }
        delay(1);
        MusicLedTimer++;
      }
      digitalWrite(13, HIGH);
    }

  }
}

void Morning() {
  wakeUpStatus = 1;
  while (wakeUpRedLEDTimer < 200) {
    wakeUpRedLEDTimer++;
    analogWrite(R, wakeUpRedLEDTimer);
    delay(10);
  }
  while (wakeUpGreenLEDTimer < 35) {
    wakeUpGreenLEDTimer++;
    analogWrite(G, wakeUpGreenLEDTimer);
    delay(15);
  }
  while (wakeUpWhiteLEDTimer < 255) {
    wakeUpWhiteLEDTimer++;
    analogWrite(W, wakeUpWhiteLEDTimer);
    delay(25);
  }
  digitalWrite(13, HIGH);
  delay(2500);
  while (wakeUpStatus == 1) {

    if (irrecv.decode(&results)) {
      Serial.println(results.value, HEX);
      irrecv.resume(); // Receive the next value
    }

    if (results.value == 0x1C345678) {
      wakeUpStatus = 0;
      results.value = 0xFFFFFFFF;
      digitalWrite(13, LOW);
      fastLamp();
    }

    if (results.value == 0x1C245678) {
      wakeUpStatus = 0;
      digitalWrite(13, LOW);
      results.value = 0x1A545678;
      fastLamp();
    }
  }
  delay(1000);
}

void loop() {

  if (results.value == 0x1A545678) { ///Fast lamp mode///
    results.value = 0xFFFFFFFF;
    fastLamp();
  }

  if (musicMode == 1) {
    Music1();
  }
  if (musicMode == 2) {
    Music2();
  }

  if (wakeUpOn == 1) {
    if (analogRead(wakeUpPin) > wakeUpTrigger) {
      Morning();
    }
  }
  
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }

//////////Decrease brightness//////////

  if (results.value == 0x1B245678) {
    if (selected == R) {
      Rval = Rval - 10;
      analogWrite(R, constrain(Rval, 0, 255));
      delay(commandDelay);
      results.value = 0xFFFFFFFF;
    }
    if (selected == G) {
      Gval = Gval - 10;
      analogWrite(G, constrain(Gval, 0, 255));
      delay(commandDelay);
      results.value = 0xFFFFFFFF;
    }
    if (selected == B) {
      Bval = Bval - 10;
      analogWrite(B, constrain(Bval, 0, 255));
      delay(commandDelay);
      results.value = 0xFFFFFFFF;
    }
    if (selected == W) {
      Wval = Wval - 10;
      analogWrite(W, constrain(Wval, 0, 255));
      delay(commandDelay);
      results.value = 0xFFFFFFFF;
    }
  }

//////////Increase brightness//////////

  if (results.value == 0x1B345678) {
    if (selected == R) {
      Rval = Rval + 10;
      analogWrite(R, constrain(Rval, 0, 255));
      delay(commandDelay);
      results.value = 0xFFFFFFFF;
    }
    if (selected == G) {
      Gval = Gval + 10;
      analogWrite(G, constrain(Gval, 0, 255));
      delay(commandDelay);
      results.value = 0xFFFFFFFF;
    }
    if (selected == B) {
      Bval = Bval + 10;
      analogWrite(B, constrain(Bval, 0, 255));
      delay(commandDelay);
      results.value = 0xFFFFFFFF;
    }
    if (selected == W) {
      Wval = Wval + 10;
      analogWrite(W, constrain(Wval, 0, 255));
      delay(commandDelay);
      results.value = 0xFFFFFFFF;
    }
  }

//////////Select color//////////

  if (results.value == 0x1B545678) {
    selected = R;
    delay(commandDelay);
    results.value = 0xFFFFFFFF;
  }
  if (results.value == 0x1B645678) {
    selected = G;
    delay(commandDelay);
    results.value = 0xFFFFFFFF;
  }
  if (results.value == 0x1B745678) {
    selected = B;
    delay(commandDelay);
    results.value = 0xFFFFFFFF;
  }
  if (results.value == 0x1B845678) {
    selected = W;
    delay(commandDelay);
    results.value = 0xFFFFFFFF;
  }

////////////Switch wakeUp mode////////////

  if (results.value == 0x1C345678) {
    wakeUpOn = 1;
    delay(commandDelay);
    results.value = 0xFFFFFFFF;
  }

  if (results.value == 0x1C245678) {
    wakeUpOn = 0;
    delay(commandDelay);
    results.value = 0xFFFFFFFF;
  }

  if (wakeUpOn == 1) {
    if (ledTimer == 5000) {
      digitalWrite(13, HIGH);
    }
    if (ledTimer == 5500) {
      digitalWrite(13, LOW);
      ledTimer = 0;
    }
    delay(1);
    ledTimer++;
  }

  if (results.value == 0x1A245678) {
    if (musicMode == 0){
      musicMode = 1;
      results.value = 0xFFFFFFFF;
      delay(500);
    }
  }

  if (results.value == 0x1A345678) {
    if (musicMode == 0){
      musicMode = 2;
      results.value = 0xFFFFFFFF;
      delay(500);
    }
  }

}
