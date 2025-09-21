/*
 * LED.c
 *
 *  Created on: Oct 4, 2024
 *      Author: Youssef Khaled
 */

#include "LED.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the LED driver by setting LED pins as output and turning them off.
 */
void LEDS_init() {
    GPIO_setupPinDirection(RED_LED_PORT, RED_LED_PIN, PIN_OUTPUT);
    GPIO_setupPinDirection(GREEN_LED_PORT, GREEN_LED_PIN, PIN_OUTPUT);
    GPIO_setupPinDirection(BLUE_LED_PORT, BLUE_LED_PIN, PIN_OUTPUT);

    GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_LOW);
    GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_LOW);
    GPIO_writePin(BLUE_LED_PORT, BLUE_LED_PIN, LOGIC_LOW);
}

/*
 * Description :
 * Turn on the specified LED.
 */
void LED_on(LED_ID id) {
    if (LED_LOGIC == POSITIVE_LOGIC) {
        switch (id) {
        case RED_LED:
            GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_HIGH);
            break;
        case GREEN_LED:
            GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_HIGH);
            break;
        case BLUE_LED:
            GPIO_writePin(BLUE_LED_PORT, BLUE_LED_PIN, LOGIC_HIGH);
            break;
        }
    } else {
        switch (id) {
        case RED_LED:
            GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_LOW);
            break;
        case GREEN_LED:
            GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_LOW);
            break;
        case BLUE_LED:
            GPIO_writePin(BLUE_LED_PORT, BLUE_LED_PIN, LOGIC_LOW);
            break;
        }
    }
}

/*
 * Description :
 * Turn off the specified LED.
 */
void LED_off(LED_ID id) {
    if (LED_LOGIC == POSITIVE_LOGIC) {
        switch (id) {
        case RED_LED:
            GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_LOW);
            break;
        case GREEN_LED:
            GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_LOW);
            break;
        case BLUE_LED:
            GPIO_writePin(BLUE_LED_PORT, BLUE_LED_PIN, LOGIC_LOW);
            break;
        }
    } else {
        switch (id) {
        case RED_LED:
            GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_HIGH);
            break;
        case GREEN_LED:
            GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_HIGH);
            break;
        case BLUE_LED:
            GPIO_writePin(BLUE_LED_PORT, BLUE_LED_PIN, LOGIC_HIGH);
            break;
        }
    }
}
