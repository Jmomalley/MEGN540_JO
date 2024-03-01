#include "Battery_Monitor.h"
#include "Filter.h"

// Found from documentation
static const float BITS_TO_BATTERY_VOLTS = 0.0046920821f;

/**
 * Function Initialize_Battery_Monitor initializes the Battery Monitor to record the current battery voltages.
 */
void Initialize_Battery_Monitor()
{

    // *** MEGN540 LAB3 YOUR CODE HERE ***
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
    ADCSRA |= ( 1 << ADSC );

    while( !( ADCSRA & ( 1 << ADIF ) ) )
        ;  // wait for ADC to complete reading

    data.split.LSB = ADCL;        // Read the low bits
    data.split.MSB = ADCH * 256;  // Read the high bits

    data.value = data.split.LSB + data.split.MSB;

    
    return 2 * data.value * BITS_TO_BATTERY_VOLTS;
    
}
