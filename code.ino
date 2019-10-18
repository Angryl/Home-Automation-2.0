#include <SoftwareSerial.h>
#include "SIM900.h"
#include "sms.h"
SMSGSM sms;

SoftwareSerial mySerial(9, 10);
SoftwareSerial bt(0,1); /* (Rx,Tx) */


/*str variable for storing input from controling devics like:mobile*/
	String str; 

	//variables Decleration
	int Fan = 8;
	int Light = 9;
	int Computer = 10;
	int Music = 11;
	int Buzzer = 13; // Use buzzer for alert 	
	int sensor=A1;
	float temp_read,Temp_alert_val,Temp_shut_val;
	int sms_count=0,Fire_Set;
	const int stop_button = 2; 
	int buttonState = 0;  //reading stop alarm button state


void setup() 
{
	bt.begin(9600);
	Serial.begin(9600);
	pinMode(sensor,INPUT);
	mySerial.begin(9600);     
	lcd.begin(16,2);  
	delay(500);
	
	pinMode(Fan,OUTPUT);
	pinMode(Light,OUTPUT);
	pinMode(Computer,OUTPUT);
	pinMode(Music,OUTPUT);
	pinMode(Buzzer, OUTPUT);
	pinMode(stop_button, INPUT);

  
}
 
 void loop() 
 {
	//GSM firealarm alerting system 
	CheckFire();
    automation();
    buttonState = digitalRead(stop_button);
		if (buttonState == HIGH) 
		{
			// alarm on
			Fire_Set=1; 
		}
	CheckShutDown(); 

   
}

void automation()
{
	 //wireless bluetooth room applience controling system  
  if (bt.available())
 {
	str = bt.read();
	Serial.println(str);
//Fan
	if(str==”Fan on”)
	{
	  digitalWrite(Fan,HIGH);
	  Serial.println(“Fan ON”);	
	}
	else if(str=="Fan off”)
	{
	  digitalWrite(Fan,LOW);
	  Serial.println(“Fan OFF”);	
	}
	else
	{
	  digitalWrite(Fan,LOW);
	}
//Light
	if(str==”Light on”)
	{
	  digitalWrite(Light,HIGH);
	  Serial.println(“Light ON”);
	}
	else if(str==”Light off”)
	{
	  digitalWrite(Light,LOW);
	  Serial.println(“Light OFF”);
	}
	else
	{
	  digitalWrite(Light,LOW);
	}
//Computer
	if(str==”Computer on”)	
	{
	  digitalWrite(Computer,HIGH);
	  Serial.println(“Computer ON”);	
	}
	else if(str==”Computer off”)
	{
	  digitalWrite(Computer,LOW);
	  Serial.println(“Computer is OFF”);	
	}
	else
	{
	  digitalWrite(Computer,LOW);
	}
//Music
	if(str==”Music on”)
	{
	  digitalWrite(Music,HIGH);
	  Serial.println(“Music ON”);
	}
	else if(str==”Music off”)
	{
	  digitalWrite(Music,LOW);
	  Serial.println(“Music OFF”);
	}
	else
	{
	  digitalWrite(Music,LOW);
	}
 }
}

 //function that detect the fire
 void CheckFire()
{
  Temp_alert_val=CheckTemp();
  if(Temp_alert_val>45)
  {
    SetAlert(); // Function to send SMS Alerts
  }
}

float CheckTemp()
{
  temp_read=analogRead(sensor); // reads the sensor output (Vout of LM35)
  temp_read=temp_read*5;    // converts the sensor reading to temperature
  temp_read=temp_read/10;  // adds the decimal point
  return temp_read; // returns temperature value in degree celsius
}

void SetAlert()
{	
  while(sms_count<3) //Number of SMS Alerts to be sent
  {  
    SendTextMessage(); // Function to send AT Commands to GSM module  
  }
  
  
  //sounds the alarms
  digitalWrite(Buzzer, HIGH);
}

void CheckShutDown()
{
  if(Fire_Set==1) 
  {

    Temp_shut_val=CheckTemp();
    if(Temp_shut_val<28)
    {
    digitalWrite(Buzzer, LOW);
    sms_count=0;
    Fire_Set=0;
    }
  }
}

void SendTextMessage()
{
  mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(2000);
  mySerial.println("AT+CMGS=\"+919544xxxxxx\"\r"); // change to the phone number you using 
  delay(2000);
  mySerial.println("Fire in HOUSE!");//the content of the message
  delay(200);
  mySerial.println((char)26);//the stopping character
  delay(5000);
   mySerial.println("AT+CMGS=\"+919847xxxxxx\"\r"); // change to the phone number you using 
  delay(2000);
  mySerial.println("Fire in HOUSE!");//the content of the message
  delay(200);
  mySerial.println((char)26);//the message stopping character
  delay(5000);
  sms_count++;
}
