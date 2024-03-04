#include "Battery_Monitor.h"

#include "led_interface.h"

static const float BITS_TO_BATTERY_VOLTS = 0.0050048876f;  // ((ADC * V_ref )/ (2^10 - 1))*2

/**
 * Function Initialize_Battery_Monitor initializes the Battery Monitor to record the current battery voltages.
 */
void Initialize_Battery_Monitor()
{

    // PORTF |= ( 1 << PORTF6 );  // PF6

    // DDRF &= ( 1 << DDE6 );  // INPUT

    ADCSRA |= ( 1 << ADEN );  // Enable the ADC

    ADCSRA |= ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );  // Set the prescaler to 128

    ADMUX |= ( 1 << MUX1 ) | ( 1 << MUX2 );  // Select ADC6 which is connected to PF6

    ADCSRB &= ( 0 << MUX5 );

    ADMUX |= ( 1 << REFS0 ) | ( 1 << REFS1 );  // Sets the Reference Voltage to 2.56V

    // ADMUX |= ( 1 << REFS0 );

    DIDR0 |= ( 1 << ADC6D );  // Disable digital inputs on ADC6
}

/**
 * Function Battery_Voltage initiates the A/D measurement and returns the result for the battery voltage.
 */
float Battery_Voltage()
{
    // A Union to assist with reading the LSB and MSB in the  16 bit register
    union {
        struct {
            uint8_t LSB;
            uint8_t MSB;
        } split;
        uint16_t value;
    } data = { .value = 0 };

    // *** MEGN540 LAB3 YOUR CODE HERE ***
    ADCSRA |= ( 1 << ADSC );  // Start the ADC measurement

    while( !( ADCSRA & ( 1 << ADIF ) ) ) {};  // wait for ADC to complete reading

    data.split.LSB = ADCL;  // Read the low bits
    data.split.MSB = ADCH;  // Read the high bits

    // ADCSRA |= ( 1 << ADIF );

    return data.value * BITS_TO_BATTERY_VOLTS;
}
