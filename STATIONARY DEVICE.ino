#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <RH_ASK.h>
#include <SPI.h>


LiquidCrystal_I2C lcd(0x27, 20, 4);
RH_ASK rf_driver;
int led = 13;
int buzzer = 12;

void setup()
{
  
  Serial.begin(9600);
  //Initialize ASK object
  rf_driver.init();
  lcd.begin();
  lcd.backlight();
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
    
  Serial.println("Setting up");  
  lcd.setCursor(7,0);
  lcd.print("SYSTEM");
  lcd.setCursor(4,1);
  lcd.print("INITIALIZING");
  lcd.setCursor(7,2);
  lcd.print("PLEASE");
  lcd.setCursor(8,3);
  lcd.print("WAIT"); 
  delay(10000); 000
  digitalWrite(led, LOW);
  digitalWrite(buzzer, LOW);

  
}

void loop()
{
  recieve_data();
}

void recieve_data()
{
  uint8_t buf[10];
  uint8_t buflen = sizeof(buf);
  if (rf_driver.waitAvailableTimeout(4000))
  {    
   if (rf_driver.recv(buf, &buflen))
   {
     Serial.print("Message Receivedd: ");
     Serial.println((char*)buf);
     lcd.clear();
     lcd.setCursor(6,0);
     lcd.print("IN RANGE");
     lcd.setCursor(1,2);
     lcd.print("WEARABLE  TRACKING");
     lcd.setCursor(7,3);
     lcd.print("DEVICE");
     digitalWrite(led, LOW);
   }  
  }
  
  else
  {
      Serial.println("No message Receive");
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("NOT IN RANGE");
      lcd.setCursor(6,1);
      lcd.print("Send SMS");
      lcd.setCursor(1,2);
      lcd.print("WEARABLE  TRACKING");
      lcd.setCursor(7,3);
       lcd.print("DEVICE");
      led_alarm();
      buzzer_alarm();
      
  }
}

void led_alarm()
{
  digitalWrite(led, HIGH);
  delay(3000);
  digitalWrite(led, LOW);
  delay(400);
}
void buzzer_alarm()
{
  digitalWrite(buzzer, HIGH);
}
