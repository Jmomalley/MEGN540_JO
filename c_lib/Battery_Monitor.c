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
//ADCSRA |= ( 1 << ADSC ); //ADC conversion is started by writing a 1 to ADSC

ADCSRA |= (1 << ADPS2); //Set prescalar to 128
ADCSRA |= (1 << ADPS1); //Set prescalar to 128
ADCSRA |= (1 << ADPS0); //Set prescalar to 128

//Set reference voltage to 2.56V
ADMUX |= (1 << REFS1); 
ADMUX |= (1 << REFS0);

//SELECT ADC6 AS THE VOLTAGE READOUT PIN
ADMUX |= (1 << MUX1) | (1 << MUX2);

DIDR0 |= (1 << ADC0D); // DISABLE DIGITIAL INPUT BUFFER. SUGGESTED ON THE LECTURE SLIDES. ASK WHY AND WHETHER ZERO IS THE RIGHT PIN

//Enable the ADC | ASK WHETHER THIS IS NECESSARY. SEEMS LIKE ITS NEGATED BY ADSC
ADCSRA |= (1 << ADEN); //enable ADC module
//ADCSRA |= (1 << ADATE);  //enable autotriggering 
//ADCSRA |= (1 << ADIE); // store ADC value and reset timer compare match flag to perform ADC conversion complete interrupt
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
