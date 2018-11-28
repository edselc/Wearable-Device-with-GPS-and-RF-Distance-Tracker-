
#include <RH_ASK.h>
#include <SPI.h>
#include <SoftwareSerial.h>
SoftwareSerial sim808(3, 2);
RH_ASK driver;
char phone_no[] = " "; // insert the phone number of the receiver inside the quotations
String data[5];
#define DEBUG true
String state, timegps, latitude, longitude, textMessage;
const int led = 13;
void setup()
{
sim808.begin(9600);
Serial.begin(9600);
delay(50);
sim808.print("AT\r");
delay(100);
sim808.print("ATE0\r");
delay(400);
sim808.print("AT+CMGF=1\r");
delay(400);
sim808.print("AT+CMGD=1,4\r");
delay(400);
sim808.print("AT+CNMI=1,2,0,0,0\r");
delay(100);
sendData("AT+CGNSPWR=1", 1000, DEBUG);
delay(50);
sendData("AT+CGNSSEQ=RMC", 1000, DEBUG);


sendTabData("AT+CGNSINF", 1000, DEBUG);


delay(150);
pinMode(led, OUTPUT);
digitalWrite(led, LOW);
if (!driver.init())
Serial.println("rf driver init failed");
}
void loop()
{
transmitter();

sim808.print("AT+CNMI=1,2,0,0,0\r");
delay(100);
if (sim808.available() > 0)
  {
    textMessage = sim808.readString();
    Serial.print(textMessage);
    delay(10);
  }
if (textMessage.indexOf("GPS") >= 0)
  {
    digitalWrite(led, HIGH);
    Serial.println("GPS");
   Send_SMS(); 

  }
}
void transmitter()
{
  const char *msg = "0123456789";
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  
  
  sendTabData("AT+CGNSINF", 1000, DEBUG);
  
  
  delay(200);
}

void Send_SMS()
{
      sendTabData("AT+CGNSINF", 1000, DEBUG);
    if (state != 0)
      {
        Serial.println("State :" + state);
        Serial.println("Time :" + timegps);
        Serial.println("Latitude :" + latitude);
        Serial.println("Longitude :" + longitude);
        sim808.print("AT+CMGS=\"");
    
        sim808.print(phone_no);
        sim808.println("\"");
        delay(300);
        sim808.print("http://maps.google.com/maps?q=");
        sim808.print(latitude);
        sim808.print(",");
        sim808.print (longitude);
        delay(200);
        sim808.println((char)26);
        delay(200);
        sim808.println();
        delay(20000);
        sim808.flush();
      }
        else
        {
           Serial.println("GPS Initializing…");
        }
}

void sendTabData(String command , const int timeout , boolean debug) 
{
  sim808.println(command);
  long int time = millis();
  int i = 0;
  while ((time + timeout) > millis())
  {
      while (sim808.available())
    {
        char c = sim808.read();
        if (c != ',')
      {
        data[i] += c;
        delay(100);
      }
        else
      {
        i++;
      }
        if (i == 5)
      {
        delay(100);
        goto exitL;
      }
    }
  }
  exitL:
  if (debug)
  {
  state = data[1];
  timegps = data[2];
  latitude = data[3];
  longitude = data[4];
  
  }
}

String sendData (String command , const int timeout , boolean debug)
{
  String response = "";
  sim808.println(command);
  long int time = millis();
  int i = 0;
  while ( (time + timeout ) > millis())
  {
      while (sim808.available())
    {
      char c = sim808.read();
      response += c;
    }
  }
    if (debug)
  {
    Serial.print(response);
  }
  return response;
}
