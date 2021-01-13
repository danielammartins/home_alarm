#include <avr/io.h>
#include <stdio.h>
#include "uartLib/uart.hpp"
#include "ultrasonicLib/HCSR04.h"

#define SAFETY 25 			//define safety perimeter as 25cm

// Serial Terminal
#define BAUD 9600
#define MYUBRR  F_CPU/16/BAUD-1

// Pins
#define trigPin PD4 		// trigger pin connected to digital pin 12
#define echoPin PD3 		// echo pin connected to digital pin 13
#define buzzerPin PD2		// buzzer pin connected to digital pin 9
#define ledRed PD6 			// Blue led connected to digital pin 7
#define ledOn PD5 			// Blue led connected to digital pin 6

/*
 * Sensor initialisation
 * HCSR04(trigger, echo, temperature, distance)
 * Assuming room temperature = 20ºC and max distance 300cm
*/
HCSR04 ultrasonicSensor(4, 3, 20, 300);

int totalDistance, distance, duration;
unsigned char str[100];

void setup() {
	// Serial terminal initialisation
	init_uart(MYUBRR);

	// Pin configurations
	DDRD |= (1 << DDD4); 					// trigger pin as output
	DDRD &= ~(1 << DDD3);					// echo pin as input
	DDRD |= (1 << DDD2); 					// buzzer pin as output
	DDRD |= ((1 << DDD5) | (1 << DDD6)); 	// led pins as output

	//ultrasonicSensor.begin(); //set trigger as output & echo pin as input - LIBRARY FUNCTION
}

void loop() {

	// Measure duration of the pulse in ms
	duration =  ultrasonicSensor.getEchoPulseLength();

	// Distance to and from the object
	totalDistance = (duration / 29);

	// Distance to the object
	distance = totalDistance / 2;

	// BUZZER ON
	if (distance > 0 && distance < SAFETY){
		PORTD |= (1 << buzzerPin);   // turn on buzzer
		PORTD |= (1 << ledRed);		// turn on red led
		PORTD &= ~(1 << ledOn);     // turn off blue led
	}
	// BUZZER OFF
	else {
		PORTD &= ~(1 << buzzerPin);  //turn off buzzer
		PORTD |= (1 << ledOn);  	// turn on blue led
		PORTD &= ~(1 << ledRed);	// turn off red led
	}


	/*
		Serial.print("Distance: ");
		Serial.println(distance);
		_delay_ms(200);
	*/
}
