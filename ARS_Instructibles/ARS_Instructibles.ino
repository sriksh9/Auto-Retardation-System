//ARS system on Wireless Remote Car

//byte rb1, rb2, rb3, rb4;
byte echoPin = 9, trigPin = 8;
byte e1 = 6, e2 = 3;//e1=>right motor enable
byte a1 = 7, b1 = 4, a2 = 5, b2 = 2;
byte arsSwitch = 10;
byte arsLed = 11;

char junk;
String inputString = "";

long microsecondsToCentimeters(long microseconds)
{

  return (microseconds / 29 / 2);
}




void setup()
{
  int i;
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  for (i = 2; i <= 7; i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(arsSwitch, INPUT);
  pinMode(arsLed, OUTPUT);

}

void loop()
{
  long duration, inches, cm, k;
  int arsswitchstate;
  int minRange = 20, maxRange = 70;

  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  if (Serial.available()) {
    while (Serial.available())
    {
      char inChar = (char)Serial.read(); //read the input
      inputString += inChar;        //make a string of the characters coming on serial
    }
    Serial.println(inputString);
    while (Serial.available() > 0)
    {
      junk = Serial.read() ;  // clear the serial buffer
    }
    if (inputString == "a") {     //in case of 'a' turn the LED on
    { 
      if (digitalRead(arsSwitch) == HIGH)
      {
        digitalWrite(arsLed, HIGH);
        // convert the time into a distance
        
        cm = microsecondsToCentimeters(duration);
        if (cm > maxRange && cm == 0)
        {
          analogWrite(e1, 255);
          analogWrite(e2, 255);
        }
        else if (cm > 0 && cm < minRange)
        {
          digitalWrite(e1, LOW);
          digitalWrite(e2, LOW);
        }
        else
        {
          k = map(cm, minRange, maxRange, 0, 235);
          analogWrite(e1, k);
          analogWrite(e2, k);
          Serial.print(k);
          Serial.print("PWM ");
          digitalWrite(a1, HIGH);
          digitalWrite(b1, LOW);
          digitalWrite(a2, HIGH);
          digitalWrite(b2, LOW);
          // last_state = k;
       }
      }
      else
      {
        digitalWrite(arsLed, LOW);
        Serial.print("ARS OFF ");
        analogWrite(e1, 255);
        analogWrite(e2, 255);
        digitalWrite(a1, HIGH);
        digitalWrite(b1, LOW);
        digitalWrite(a2, HIGH);
        digitalWrite(b2, LOW);
      }
    }
    }
    else if (inputString == "b")//RIGHT TURN
    {
     analogWrite(e1, 100);
     analogWrite(e2, 250);
     digitalWrite(a1, HIGH);
     digitalWrite(b1, LOW);
     digitalWrite(a2, HIGH);
     digitalWrite(b2, LOW);
    }
    else if (inputString == "c")//LEFT TURN
    {
      analogWrite(e1, 250);
      analogWrite(e2, 100);
      digitalWrite(a1, HIGH);
      digitalWrite(b1, LOW);
      digitalWrite(a2, HIGH);
      digitalWrite(b2, LOW);
    }
    else if (inputString == "d")//Reverse
    {
      analogWrite(e1, 250);
      analogWrite(e2, 250);
      digitalWrite(a1, LOW);
      digitalWrite(b1, HIGH);
      digitalWrite(a2, LOW);
      digitalWrite(b2, HIGH);
    }
    
    else
    {
      analogWrite(e1, 0);
      analogWrite(e2, 0);
    }
    inputString = "";
  }
}





