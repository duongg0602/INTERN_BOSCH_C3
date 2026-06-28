#include <Arduino.h>
#define LED 2
char c[10];
void setup() {
  // put your setup code here, to run once:
  	pinMode(LED, OUTPUT);
  	Serial.begin(115200);
}

void loop() {
	
	if(Serial.available()){
		for(int i = 0; i < 9; i++){
			c[i] = Serial.read();
		}
		while(c[0] != 'O');
		if(c[1] == 'N'){
			digitalWrite(LED, HIGH);
		}
		if(c[1] == 'F'){
			if(c[2] == 'F'){
				digitalWrite(LED, LOW);
			}
		}		
	}
}

