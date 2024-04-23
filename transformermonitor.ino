Arduino program
//Library version:1.1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27,20,4);
SoftwareSerial sim(10, 11);
int _timeout;
String _buffer;
String number = "+9779811929591";  


int delayTime = 1000;

float temp;
float TempSensorPin = A0;
float vout;
int b;
int c;
int d;
int e;

const int trigPin = 8; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 7; // Echo Pin of Ultrasonic Sensor
long duration;
float OilLevel;


const int sensorIn = A1;
int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module

double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;


void setup() {

   // put your setup code here, to run once:
pinMode(TempSensorPin,INPUT);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

Serial.begin(9600);
_buffer.reserve(50);
Serial.println("Sistem Started...");
sim.begin(9600);
delay(1000);
lcd.init();                      // initialize the lcd
lcd.init();
  // Print a message to the LCD.
lcd.backlight();
  // put your setup code here, to run once
 
 delay(100);

lcd.print("PROJECT WORK");
lcd.setCursor(2,1);
lcd.print("REMOTE");
lcd.setCursor(9,1);
lcd.print("MONITORING");
lcd.setCursor(4,2);
lcd.print("TRANSFORMER");
delay (delayTime);
lcd.clear();

}

void loop()
{//temperature//
vout = analogRead(TempSensorPin);
vout = (vout * 500) / 1023;
temp = vout;

if(temp  <= 35)
{b=0;}
if (OilLevel < 5.4)
{d=0;}
if (OilLevel > 4.4)
{c=0;}
if(AmpsRMS <= 0.67)
{
  e=0;
}

Voltage = getVPP();
VRMS = (Voltage/2.0) *0.707; //root 2 is 0.707
AmpsRMS = (VRMS * 1000)/mVperAmp;   //Calculate the current value for earth
Serial.print(AmpsRMS);
Serial.println("Amps RMS");


// SENSING OIL LEVEL//
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);

OilLevel = duration * 0.034 / 2;

lcd.setCursor(0,0);
lcd.print("Temperature:");
lcd.setCursor(12,0);
lcd.print(temp);
lcd.setCursor(17,0);
lcd.print("C");

lcd.setCursor(0,1);
lcd.print("Oil Level:");
lcd.setCursor(10,1);
lcd.print(OilLevel);
lcd.setCursor(14,1);
lcd.print("CM");
delay(1000);



lcd.setCursor(0, 2);
lcd.print("Current:");
lcd.setCursor(14, 2);
lcd.print(AmpsRMS,2);
lcd.setCursor(18, 2);
lcd.print("A");
delay(600);



condition();
if (sim.available())
    Serial.write(sim.read());
 
  sim.println("AT");
  delay(1000);
 
  sim.println("AT+CPIN?");
  delay(1000);
 
  sim.println("AT+CREG?");
  delay(1000);
 
  sim.println("AT+CGATT?");
  delay(1000);
 
  sim.println("AT+CIPSHUT");
  delay(1000);
 
  sim.println("AT+CIPSTATUS");
  delay(2000);
 
  sim.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  sim.println("AT+CSTT=\"Ntnet\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  sim.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  sim.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  sim.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
 
  sim.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  ShowSerialData();
 
  sim.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
 
  String str="GET https://api.thingspeak.com/update?api_key=PKPASH6V66N282P9&field1="+String(temp)+"&field2="+String(AmpsRMS)+"&field3="+String(OilLevel) ;
  Serial.println(str);
  sim.println(str);//begin send data to remote server
 
  delay(4000);
  ShowSerialData();
 
  sim.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet
  sim.println();
 
  ShowSerialData();
 
  sim.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();


}
long microsecondsToCentimeters(long microseconds)
{
return microseconds / 29 / 2;}
void ShowSerialData()
{
  while(sim.available()!=0)
  Serial.write(sim.read());
  delay(5000);
 
}
float getVPP()
{
float result;
int readValue; //value read from the sensor
int maxValue = 0; // store max value here
int minValue = 1024; // store min value here

uint32_t start_time = millis();
while((millis()-start_time) < 3000) //sample for 3 Sec
{
readValue = analogRead(sensorIn);
// see if you have a new maxValue
if (readValue > maxValue)
{
/*record the maximum sensor value*/
maxValue = readValue;
}
if (readValue < minValue)
{
/*record the minimum sensor value*/
minValue = readValue;
}}
// Subtract min from max
result = ((maxValue - minValue) * 5.0)/1024.0;
return result;
}

void SendMessage()
{
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1"); //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "high temperature";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();
}


void SendMessage1()
{
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1"); //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "Oil level High";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();
}
void SendMessage2()
{
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1"); //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "low oil level";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();
}
void SendMessage3()
{
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1"); //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "Over current ";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();
}
String _readSerial() {
  _timeout = 0;
  while (!sim.available() && _timeout < 12000 )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}

void callNumber() {
  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
  _buffer = _readSerial();
  Serial.println(_buffer);
}

void condition()
{
  if (temp > 36 and b==0)
{
 
 lcd_tempPrint();
 //callNumber();
 SendMessage();
 b++;
  delay(5000);}
   if (OilLevel < 4.4 and c==0 )
{
  lcd_oilLevelFULLPrint();
  SendMessage1();
    c++;
 delay(5000);}
 if (OilLevel > 5.4 and d==0){
   lcd_oilLevelLOWPrint();
   d++;
 }
 if (AmpsRMS>0.67 and e==0)
{
  lcd_earthPrint();
 SendMessage3();
 e++;
delay(5000);
}  }
void lcd_tempPrint()
{
lcd.clear();
lcd.setCursor(0,0);
lcd.clear();                                  //if condition temp for it to print.
lcd.print("HIGH TEMPERATURE");  
 delay(5000);
}
void lcd_oilLevelFULLPrint()
{
lcd.clear();
lcd.setCursor(1,1);
lcd.clear();                                  //if condition temp for it to print.
lcd.print("OIL TANK FULL");  
 delay(5000);
}
void lcd_oilLevelLOWPrint()
{
lcd.clear();
lcd.setCursor(0,2);
lcd.clear();                                  //if condition temp for it to print.
lcd.print("OIL LEVEL LOW");  
delay(5000);
}
void lcd_earthPrint()
{
lcd.clear();
lcd.setCursor(0,0);
lcd.clear();                                  //if condition temp for it to print.
lcd.print("Over current");  
 delay(5000);
}
