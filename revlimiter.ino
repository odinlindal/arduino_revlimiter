//heavily inspired by korn101
//https://github.com/korn101
//adapted by Odin Lindal to include adjustment by potmeter, and car-specific adjustments
//tested on 93' Toyota Celica 3S-GT engine

#include <EEPROM.h>

//constant values, for pin-numbers
const byte ledPin = 13;
const byte interruptPin = 2;
const byte relayPin = 3;
const byte led1 = 5;
const byte button1 = 6;
const byte led2 = 7;
const byte button2 = 8;
const byte lcButton = 9;

volatile byte state = LOW;
byte enabled = true;
int lastStart = 0;
int lastEnd = 0;
int betweenStarts = 0;
int betweenEnd = 0;
int pulseWidth = 0; // Pulse width of the pulse.
float rpm = 0;
float rpmEnds = 0;
float rpmAverage = 0;
float rpmEndsAverage = 0;

volatile int lastFall;
volatile int betweenFalls;
volatile float rpmFalls = 0;
volatile float rpmFallsAverage = 0;

float revLimitRpm = 3000; // initial RPM value, will be overwritten if EEPROM contains old value.
int rpmSmoothingConst = 10;
int responseDelay = 20; // 20 - fast
int returnDelay = 20; // delay between spark cut and start, magnified with cutHarshnessFactor.
float revLimitSafetyOffset = 3000; // rpm above rev limit where spark is default allowed, helps for in case handbrake is pulled at speed.

float cutHarshnessFactor = 1;
int sensorValue = 0;
float sensorValue1 = 0;

void setup() {
  Serial.begin(115200);
  // SETUP PINS:
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP); // or INPUT_PULLUP
  pinMode(relayPin, OUTPUT);
  // Set up IO buttons / leds
  pinMode(led2, OUTPUT);
  // Launch control button, enables LC momentarily:
  pinMode(lcButton, INPUT_PULLUP);
  // Potentiometers to adjust Rev limit and Harshness
  pinMode(A0, INPUT); // rev limit potmeter
  pinMode(A1, INPUT); // harshness potmeter
  // Setup Interrupts
  attachInterrupt(digitalPinToInterrupt(interruptPin), intFalling, FALLING);
  // Allow ignition by default
  digitalWrite(relayPin, LOW); // allow ignition.
  delay(1000);

}

void loop() {  
  
  sensorValue = analogRead(A0); // reads potmeter
  int mappedValue = map(sensorValue, 0, 1023, 15, 40);
  revLimitRpm = (mappedValue*100); // adjusts rev limit in regard to potmeter input
  //Serial.println(revLimitRpm);
  
  sensorValue1 = analogRead(A1); // reads potmeter
  float mappedValue1 = map(sensorValue1, 0, 1023, 100, 500);
  cutHarshnessFactor = (mappedValue1/100); // adjusts harshness in regard to potmeter input
  //Serial.println(cutHarshnessFactor);
  
  
  // If the system is enabled and the LC button is being held, then limit revs:
  if (digitalRead(lcButton) == LOW){
    if (rpmFallsAverage >= revLimitRpm && rpmFallsAverage < revLimitRpm + revLimitSafetyOffset){
      cutSpark();
      delay(returnDelay * cutHarshnessFactor);
      allowSpark();

    }
    else{
      allowSpark();
    }
  }


  delay(responseDelay);

}

void cutSpark(){ // function to open relay, cut spark
  digitalWrite(relayPin, HIGH);
  digitalWrite(led2, HIGH);
}

void allowSpark(){ // function to close relay, allow spark
  digitalWrite(relayPin, LOW);
    digitalWrite(led2, LOW);
}


void intFalling(){
  // we know the IGT line has fallen from High->Low
  // It's important to note that at 7500 rpm, the falling gap (between falling edges) should never be below 4ms.
  betweenFalls = millis() - lastFall; // measure time between this and the last fall.
  lastFall = millis();
  if (betweenFalls >= 4 && betweenFalls <= 300){
    // calculate rpm, since 2 pulses = 1 revolution of the crank. 1/(pulse gap) = pulses per second
    // Since 2 pulses occur per revolution, rpm = 2(pulses per second). 60/2 = 30. If using ms as units, it becomes 30,000.
    rpmFalls = (float) 30000.0 / betweenFalls; 
    rpmFallsAverage = (float) ( rpmFallsAverage * (rpmSmoothingConst - 1) + rpmFalls ) /rpmSmoothingConst; 
  }

}

void intChanged() {
  state = digitalRead(interruptPin);
  if (state == HIGH){
    betweenStarts = micros() - lastStart;
    lastStart = micros();
    // rpm = (float) -125.0 * ((betweenStarts / 1000.0) - 30.4); // old calculation with fitting.
    rpm = (float) 30000.0 / (betweenStarts / 1000.0);
    rpmAverage = ( rpmAverage * (rpmSmoothingConst - 1) + rpm ) /rpmSmoothingConst;     // it requires a few loops for average to get near "a"
  }

  if (state == LOW){
    betweenEnd = micros() - lastEnd;
    lastEnd = micros();
    pulseWidth = micros() - lastStart;
    rpmEnds = (float) 30000.0 / (betweenEnd / 1000.0);
    rpmEndsAverage = ( rpmEndsAverage * (rpmSmoothingConst - 1) + rpmEnds ) /rpmSmoothingConst; 
  }
}
