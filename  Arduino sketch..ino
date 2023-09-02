#include <SoftwareSerial.h>
SoftwareSerial Blue(3, 2);

long int data;
// MOTOR1 PINS
int in1 = 10;
int in2 = 11;
// MOTOR2 PINS
int in3 = 5;
int in4 = 6;

long int password1 = 92; // Start
long int password2 = 79; // Stop
int Relay = 4;

bool isStarted = false; // Flag to indicate if the motor is started
bool isPump=true;
bool isPump4=true;
int isPump8=true;
int receivedtimer;
void setup()
{
  stop();

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(Relay, OUTPUT);

  Serial.begin(9600);
  Blue.begin(9600);
}

void loop()
{

  if (Serial.available() > 0)
  {
    char command = Serial.read();
    if (command == '1')
    {
      stop();
      waterpump();
      move();
      isStarted = true;
    }
    Serial.println(command);
  }

  if (Blue.available() > 0)
  {
    data = Blue.parseInt();
    
  }
  delay(400);

  if (isStarted)
  {
    if (data == password2)
    {
      stop();
      Serial.println("stop");
      isStarted = false;
    }
  }
  else
  {
    if (data == password1)
    {
      move();
      Serial.println("start");
      isStarted = true;
    }
  }
  if(isPump){
    if(data==2){
      waterpump();
      isPump=false;
    }
  }
     if(isPump4){
    if(data==4){
      waterpump();
      isPump4=false;
    }
     }
    if(isPump8){
    if(data==8){
      waterpump();
      isPump8=false;
    }
  }
}

void move()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);
}

void stop()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void waterpump()
{
  // Process the received timer value
  // Example: Print the received value to the Serial monitor
  Serial.println(data);
  switch (data)
  {
    case 2: // watering time 2s
      digitalWrite(Relay, HIGH);
      delay(2000);
      digitalWrite(Relay, LOW);
      break;
    case 4: // watering time 4s
      digitalWrite(Relay, HIGH);
      delay(4000);
      digitalWrite(Relay, LOW);
      break;
    case 8: // watering time 30s
      digitalWrite(Relay, HIGH);
      delay(8000);
      digitalWrite(Relay, LOW);
      break;
    default:
      digitalWrite(Relay, LOW);
  }
}
