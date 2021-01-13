#include <Arduino.h>
#include <avr/io.h>
#include "uartLib/uart.hpp"
#include "ultrasonicLib/HCSR04.h"

#define SAFETY 25 			//define safety perimeter as 25cm

// Serial Terminal
#define BAUD 9600
#define MYUBRR  F_CPU/16/BAUD-1

// Pins
#define trigPin PB4 		// trigger pin connected to digital pin 12
#define echoPin PB5 		// echo pin connected to digital pin 13
#define buzzerPin PB1		// buzzer pin connected to digital pin 9
#define ledRed PD7 			// Blue led connected to digital pin 6
#define ledOn PD6 			// Blue led connected to digital pin 6

/*
 * Sensor initialisation
 * HCSR04(trigger, echo, temperature, distance)
 * Assuming room temperature = 20ºC and max distance 300cm
*/
HCSR04 ultrasonicSensor(12, 13, 20, 300);

int totalDistance, distance, duration;
unsigned char str[100];

void setup() {
	// Serial terminal initialisation
	init_uart(MYUBRR);

	// Pin configurations
	DDRB |= (1 << DDB4); 					// trigger pin as output
	DDRB &= ~(1 << DDB5);					// echo pin as input
	DDRB |= (1 << DDB1); 					// buzzer pin as output
	DDRB |= ((1 << DDD6) | (1 << DDD7)); 	// led pins as output

	//ultrasonicSensor.begin(); //set trigger as output & echo pin as input - LIBRARY FUNCTION
}

void loop() {

	// Measure duration of the pulse in ms
	duration =  ultrasonicSensor.getEchoPulseLength();

	/*
	 * Sound travels at roughly 343 m/s, meaning one centimetre takes 29.155 microseconds
	 * distance is speed / time, and if 29.155 microseconds equal 1cm, then distance = duration / 29
	 * because duration is the time the pulse takes to go to and back from the object,
	 * it needs to be divided by 2 to get just the distance to the object
	 */

	// Distance to and from the object
	totalDistance = (duration / 29);

	// Distance to the object
	distance = totalDistance / 2;

	// BUZZER ON
	if (distance > 0 && distance < SAFETY){
		PORTB |= (1 << buzzerPin);   // turn on buzzer
		PORTD |= (1 << ledRed);		// turn on red led
		PORTD &= ~(1 << ledOn);     // turn off blue led
	}
	// BUZZER OFF
	else {
		PORTB &= ~(1 << buzzerPin);  //turn on buzzer
		PORTD |= (1 << ledOn);  	// turn on blue led
		PORTD &= ~(1 << ledRed);	// turn off red led
	}

  Serial.print("Distance in centimeters: ");
  Serial.println(distance);
  delay(200);
}
