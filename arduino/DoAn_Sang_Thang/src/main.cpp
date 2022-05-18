#include <Arduino.h>

int in1 = 8; 
int in2 = 9;
int in3 = 10; 
int in4 = 11;  

void setup() 
{
   pinMode(in1, OUTPUT);
   pinMode(in2, OUTPUT);
   pinMode(in3, OUTPUT);
   pinMode(in4, OUTPUT);
} 

void motorA() 
{
    for (int i = 0; i < 34; i++) {
		digitalWrite(in1, HIGH);
    	digitalWrite(in2, LOW);
		digitalWrite(in3, LOW);
		digitalWrite(in4, LOW);
		delay(100);

		digitalWrite(in1, LOW);
    	digitalWrite(in2, HIGH);
		digitalWrite(in3, LOW);
		digitalWrite(in4, LOW);
		delay(100);

		digitalWrite(in1, LOW);
    	digitalWrite(in2, LOW);
		digitalWrite(in3, HIGH);
		digitalWrite(in4, LOW);
		delay(100);

		digitalWrite(in1, LOW);
    	digitalWrite(in2, LOW);
		digitalWrite(in3, LOW);
		digitalWrite(in4, HIGH);
		delay(100);
	}
} 

void loop() 
{
   motorA();
   delay(1000);   
}