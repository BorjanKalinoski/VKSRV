#include <ESP8266WiFi.h>
#include <EMailSender.h>
uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;

const char* email="VKSRVsmartmailbox@gmail.com";
const char* emailPassword="vksrv2019";
EMailSender emailSend(email, emailPassword);

bool emailSent = false;

const char* ssid = "phone";
const char* password = "12345678";

const int trigPin1 = 2;  //D4
const int echoPin1 = 0;  //D3

const int trigPin2 = 14;  //D5
const int echoPin2 = 12;  //D6

long duration;
float distance;
#define MAX_DISTANCE 200//cm max-400/500
// define the timeOut according to the maximum range. timeOut= 2*MAX_DISTANCE /100 /340
//*1000000 = MAX_DISTANCE*58.8
float timeOut=MAX_DISTANCE*60;
int soundVelocity=340;//speed of air =const = 340ms

void setup() {
  Serial.begin(9600); 
  Serial.print("connecting to ");
  Serial.println(ssid);
  Serial.print("...");
  WiFi.begin(ssid, password);
  int i=0;
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(++i);
    Serial.print(" ");
  }
  Serial.println("CONNECTED ");


    
  pinMode(trigPin1, OUTPUT); // Sets the trigPin1 as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin1 as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin2 as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin2 as an Input
//// Starts the serial communication
}

void loop() {
  delay(500);

  //Serial.print("Distance: ");
  //Serial.println(getSonar());

  if((mailRecieved(getSonar(trigPin1, echoPin1)) || mailRecieved(getSonar(trigPin2, echoPin2))) && emailSent == false)
  {
    EMailSender::EMailMessage message;
    message.subject = "Soggetto";
    message.message = "Ciao come stai<br>io bene.";
    EMailSender::Response resp = emailSend.send(email, message);

    emailSent = true;

    Serial.println("Sending status: ");

    Serial.println(resp.status);
    Serial.println(resp.code);
    Serial.println(resp.desc);
  }

  else
  {
    emailSent = false;
  }
}

float getSonar(int trigPin, int echoPin){
  unsigned long pingTime;
  float distance;
  digitalWrite(trigPin,HIGH);// make trigPin output high level lasting for 10μs to triger HC_SR04
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  pingTime=pulseIn(echoPin,HIGH,timeOut); // Wait HC-SR04 returning to the high level and measure out this waitting time
  distance=(float)pingTime*soundVelocity/2/10000;//calculate the distance
  Serial.println(distance);
  Serial.println(trigPin);
  return distance;
}

bool mailRecieved(float distance)
{
  if(distance < 5 || distance > 150) 
    return true;
   return false;  
}

