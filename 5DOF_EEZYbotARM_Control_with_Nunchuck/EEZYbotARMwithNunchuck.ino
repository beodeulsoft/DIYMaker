/**************************************************************
* 5 DOF EEZYbotARM control with nunchuck ver1.0
* Arduino UNO + EEZYbotARM ( 5 servo ) + Wii Nunchuck
* Nunckuck library : https://github.com/GabrielBianconi/ArduinoNunchuk
* programed by beodeulpiri
* http://cafe.naver.com/diymaker
**************************************************************/

#include <Wire.h>
#include <ArduinoNunchuk.h>
#include <Servo.h>

#define BAUDRATE 9600

ArduinoNunchuk nunchuk = ArduinoNunchuk();

const int SERVOS = 5;
int PIN[SERVOS], value[SERVOS], idle[SERVOS], currentAngle[SERVOS], MIN[SERVOS], MAX[SERVOS], INITANGLE[SERVOS];
Servo myservo[SERVOS];

int DELAYTIME = 200;

void setup()
{
	Serial.begin(BAUDRATE);
	nunchuk.init();

	PIN[0] = 11;
	MIN[0] = 5;
	MAX[0] = 179;
	INITANGLE[0] = 90;
	PIN[1] = 10;
	MIN[1] = 5;
	MAX[1] = 179;
	INITANGLE[1] = 90;
	PIN[2] = 9;
	MIN[2] = 5;
	MAX[2] = 179;
	INITANGLE[2] = 90;
	PIN[3] = 6;
	MIN[3] = 5;
	MAX[3] = 179;
	INITANGLE[3] = 90;
	PIN[4] = 5;
	MIN[4] = 5;
	MAX[4] = 179;
	INITANGLE[4] = 45;

	for (int i = 0; i < SERVOS; i++){
		myservo[i].attach(PIN[i]);
		myservo[i].write(INITANGLE[i]);
		delay(DELAYTIME); 
		myservo[i].detach();
		value[i] = INITANGLE[i];
		idle[i] = 0;
	}
}

void loop()
{
	nunchuk.update();

	for (int i = 0; i < SERVOS; i++){
		switch(i)
		{
		case 0: 
			if(nunchuk.cButton)
				value[i] = map(nunchuk.accelX, 690, 280, MIN[i], MAX[i]); 
			else
				continue;
			break;
		case 1: 
			if(nunchuk.cButton)
				value[i] = map(nunchuk.accelY, 340, 690, MIN[i], MAX[i]); 
			else
				continue;
			break;
		case 2: 
			if(!nunchuk.cButton)
				value[i] = map(nunchuk.analogY, 230, 20, MIN[i], MAX[i]) ; 
			else
				continue;
			break;
		case 3: 
			if(!nunchuk.cButton)
				value[i] = map(nunchuk.analogX, 20, 230, MIN[i], MAX[i]); 
			else
				continue;
			break;
		case 4: 
			if(!nunchuk.cButton)
			{
				if(nunchuk.zButton)
					value[i] = 25;
				else
					value[i] = 45;
			}
			else
				continue;
			break;
		}
		currentAngle[i] = myservo[i].read();

		Serial.print(" i : ");
		Serial.print(i, DEC);
		Serial.print(" , value : ");
		Serial.print(value[i], DEC);
		Serial.print(" , curAngle : ");
		Serial.println(currentAngle[i], DEC);

		if (value[i] > MIN[i] && value[i] < MAX[i]) {
			idle[i] = 0;
			if (!myservo[i].attached())
				myservo[i].attach(PIN[i]);
			//        if(value[i] > currentAngle[i])
			//          currentAngle[i] = currentAngle[i] + 1;
			//       else
			//          currentAngle[i] = currentAngle[i] - 1;
			//      myservo[i].write(currentAngle[i]); 
			if(value[i] > currentAngle[i] + 5 || value[i] < currentAngle[i] - 5)
			{
				myservo[i].write(value[i]);  
				delay(DELAYTIME); 
				myservo[i].detach();
			}

		} else {
			++idle[i];
		}
		if (idle[i] > 10){
			myservo[i].detach();
			idle[i] = 0;
		}  
	}  

	delay(30);
}
