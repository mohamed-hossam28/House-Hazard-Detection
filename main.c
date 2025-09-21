/*
 * main.c
 *
 *  Created on: May 10, 2025
 *      Author: Joe
 */

#include "std_types.h"
#include "common_macros.h"
#include "GPIO.h"
#include "LCD.h"
#include "ADC.h"
#include "Buzzer.h"
#include "LED.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Gas_Sensor.h"

#define MAYASA 1

/* Flag for Clear LCD */
volatile uint8 LCD_CLEAR_FLAG = 0;

/* Default initial Ro For Smoke Sensor */
float Ro = 10;

/* Flag for Mode */
volatile uint8 Safety_Mode = 0;

/* Function for Interrupt 0 Initialization */
void INT0_init() {
	SREG |= (1 << 7);
	MCUCR |= (1 << ISC01);
	GICR |= (1 << INT0);
}
/* Function for Interrupt 1 Initialization */
void INT1_init() {
	SREG |= (1 << 7);
	MCUCR |= (1 << ISC11);
	GICR |= (1 << INT1);
}
ISR(INT0_vect) {
	LCD_CLEAR_FLAG = 1;
	Safety_Mode = 1;
}
ISR(INT1_vect) {
	LCD_CLEAR_FLAG = 1;
	Safety_Mode = 0;
}
int main(void) {
	/* Interrupt 0 Initialization */
	INT0_init();
	/* Interrupt 1 Initialization */
	INT1_init();

	GPIO_setupPinDirection(PORTC_ID, PIN3_ID, PIN_OUTPUT);

	/* Variable to store Temp vaule */
	uint8 Temp_value = 0;
	/* Initialize LCD */
	LCD_init();

	/* Initialize LEDS on Kit */
	LEDS_init();

	LCD_displayString("Hello ENG Taqi");
	LCD_moveCursor(1,0);
	LCD_displayString("We are MAYASA");
	_delay_ms(1000);
	/* Initialize and Config ADC */
	ADC_ConfigType config = { AVCC, ADC_CHANNEL_0, PRESCALER_128,
			ADC_RIGHT_ADJUST, POLLING };
	ADC_init(&config);

	/* Initialize Buzzer on Kit */
	Buzzer_init();

	/* ################################################### */
	/*                    Pins For DC FAN                  */
	GPIO_setupPinDirection(PORTB_ID, PIN1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTC_ID, PIN4_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTC_ID, PIN6_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTC_ID, PIN7_ID, PIN_OUTPUT);
	/* ################################################### */

	/* Initialize Buttons For Choose Mode */
	GPIO_setupPinDirection(PORTD_ID, PIN2_ID, PIN_INPUT);
	GPIO_setupPinDirection(PORTD_ID, PIN3_ID, PIN_INPUT);

	/* Initialize IR Sensor Pin*/
	GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_INPUT);

	/* Clear the LCD and display calibration message */
	LCD_clearScreen();
	LCD_displayString("Calibrating...");
	_delay_ms(1000);

	// Perform sensor calibration in clean air
	Ro = SensorCalibration();  /* Calibrate Ro */
	LCD_clearScreen();
	LCD_displayString("Ro = ");
	LCD_intgerToString((uint16) (Ro * 100)); /* Display Ro (scaled for the LCD) */

	_delay_ms(2000);  /* Wait for 2 seconds */
	LCD_clearScreen();  /* Clear the screen before starting the main loop */

	while (MAYASA) {
		/* If Safety Mode (Protection) Enabled */
		while (Safety_Mode) {
			if (LCD_CLEAR_FLAG == 1) {
				LCD_clearScreen();
				LCD_CLEAR_FLAG = 0;
			}
			LCD_displayString("Protection Mode      ");
			LCD_moveCursor(1, 0);
			LCD_displayString("                 ");
			GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_LOW);
			GPIO_writePin(PORTC_ID, PIN4_ID, LOGIC_LOW);
			GPIO_writePin(PORTC_ID, PIN6_ID, LOGIC_LOW);
			GPIO_writePin(PORTC_ID, PIN7_ID, LOGIC_LOW);
			/* If Detect Motion in House while Protection mode */
			while (!GPIO_readPin(PORTB_ID, PIN5_ID)) {
				LCD_moveCursor(1, 0);
				LCD_displayString("Haraamyyy        ");
				Buzzer_on();
				LED_on(RED_LED);
				LED_on(BLUE_LED);
				LED_on(GREEN_LED);
				_delay_ms(500);
				LED_off(RED_LED);
				LED_off(BLUE_LED);
				LED_off(GREEN_LED);
				_delay_ms(500);
			}
			Buzzer_off();
		}
		while (!Safety_Mode) {
			if (LCD_CLEAR_FLAG == 1) {
				LCD_clearScreen();
				LCD_CLEAR_FLAG = 0;
			}
			LCD_moveCursor(0, 0);
			Temp_value = LM35_getTemperature();

			/* Read sensor resistance and calculate the smoke concentration */
			float Rs = ReadSensor();
			/* Calculate the Ratio (Sensitivity) To Compare with normal Values */
			float ratio = Rs / Ro;
			int smoke_ppm = GetGasPercentage(ratio, SMOKE);

			/* Display smoke concentration on LCD */
			LCD_moveCursor(0, 0);
			LCD_displayString("Smoke: ");
			LCD_intgerToString(smoke_ppm); // Display the calculated smoke concentration
			LCD_displayString("%        ");

			/* Activate buzzer if smoke concentration exceeds threshold */
			if (smoke_ppm > 70) {
				Buzzer_on();
			} else {
				Buzzer_off();
			}
			LCD_moveCursor(1, 0);
			LCD_displayString("Temp = ");
			LCD_intgerToString(Temp_value);
			LCD_displayCharacter('C');
			LCD_displayString("      ");
			if (Temp_value > 30) {
				GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_HIGH);
				GPIO_writePin(PORTC_ID, PIN4_ID, LOGIC_HIGH);
				GPIO_writePin(PORTC_ID, PIN6_ID, LOGIC_HIGH);
				GPIO_writePin(PORTC_ID, PIN7_ID, LOGIC_HIGH);
				_delay_ms(500);
			} else {
				GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_LOW);
				GPIO_writePin(PORTC_ID, PIN4_ID, LOGIC_LOW);
				GPIO_writePin(PORTC_ID, PIN6_ID, LOGIC_LOW);
				GPIO_writePin(PORTC_ID, PIN7_ID, LOGIC_LOW);
			}
			if(Temp_value > 50){
				Buzzer_on();
				Temp_value = LM35_getTemperature();
				LCD_clearScreen();
				LCD_displayString("  Haree2aaaa   ");
				while(Temp_value > 50);
				Buzzer_off();
				LCD_clearScreen();
			}
		}

	}
}

